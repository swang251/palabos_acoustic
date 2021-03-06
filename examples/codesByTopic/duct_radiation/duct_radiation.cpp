#include "palabos3D.h"
#include "palabos3D.hh"
#include <vector>
#include <cmath>

using namespace plb;
using namespace plb::descriptors;
using namespace std;


typedef double T;
typedef Array<T,3> Velocity;
//#define DESCRIPTOR descriptors::D3Q27Descriptor
#define DESCRIPTOR MRTD3Q19Descriptor
 typedef MRTdynamics<T,DESCRIPTOR> BackgroundDynamics;
 typedef AnechoicMRTdynamics<T,DESCRIPTOR> AnechoicBackgroundDynamics;

// ---------------------------------------------
// Includes of acoustics resources
#include "acoustics/acoustics3D.h"
using namespace plb_acoustics_3D;
// ---------------------------------------------

const T rho0 = 1;
const T drho = rho0/10;

void writeGifs(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint iter){
    const plint nx = lattice.getNx();
    const plint ny = lattice.getNy();
    const plint nz = lattice.getNz();

    const plint imSize = 600;
    ImageWriter<T> imageWriter("leeloo");
    
    Box3D slice(0, nx-1, 0, ny-1, nz/2, nz/2);
    //imageWriter.writeGif(createFileName("u", iT, 6),
     //*computeDensity(lattice), );

    imageWriter.writeGif( createFileName("rho", iter, 6),
    *computeDensity(lattice, slice), 
    (T) rho0 - drho/1000000, (T) rho0 + drho/1000000, imSize, imSize);
}

void writeVTK(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint iter){
        VtkImageOutput3D<T> vtkOut(createFileName("vtk", iter, 6), 1.);
        vtkOut.writeData<float>(*computeDensity(lattice), "density", 1.);
        vtkOut.writeData<3,float>(*computeVelocity(lattice), "velocity", 1.);
}

int main(int argc, char **argv){
    plbInit(&argc, &argv);
    std::string fNameOut = "tmp";

    const T radius = 30;
    const T diameter = 2*radius;

    const plint nx = 23*diameter;
    const plint ny = 20*diameter;
    const plint nz = 20*diameter;
    const T lattice_speed_sound = 1/sqrt(3);
    const T cs2 = lattice_speed_sound*lattice_speed_sound;

    const T omega = 1.985;
    const plint maxT = 10000;
    const plint maxT_final_source = maxT - 1000;
    const T ka_max = 1.82;
    const T ka_min = 0;

    Array<T,3> u0(0, 0, 0);

    global::directories().setOutputDir(fNameOut+"/");

    //Build geometry
    Array<T,3> centerLB(30, ny/2, nz/2);
    //TriangleSet<T> triangleSet("Duto_Fechado.STL");
    //Array<T,3> center(param.cx, param.cy, param.cz);
    TriangleSet<T> triangleSet;
    pcout << "construindo o duto" << std::endl;
    /*
    (Array<T,3> const& inletCenter, T externRadius, T internRadius,
                                 T length, plint nAxial, plint nCirc)
                                 */
    T thickness_duct = 4;
    T size_duct = 11*diameter;
    triangleSet = constructDuct(centerLB, radius + thickness_duct, radius
    , (T) size_duct, (plint) 10, (plint) 50);

    //triangleSet = constructSphere<T>(centerLB, (T) 10, (plint)40);
    /*TriangleSet<T> triangleSet("duto_fechado.STL");
    triangleSet.translate(centerLB);*/
    // rotate: Z, X, Y in radians
    //triangleSet.rotate((T) 0, (T) 0, (T) 0);       
    //triangleSet.scale((T) 3.95253e+2);

    //   tube to more efficient data structures that are internally used by palabos.
    //   The TriangleBoundary3D structure will be later used to assign proper boundary conditions.
    plint borderWidth = 1;  // Because the Guo boundary condition acts in a one-cell layer.
                        // Requirement: margin>=borderWidth.
    plint margin      = 1;  // Extra margin of allocated cells around the obstacle.
    plint extraLayer  = 0;  // Make the bounding box larger; for visualization purposes
                        //   only. For the simulation, it is OK to have extraLayer=0.
    plint xDirection = 0;
    DEFscaledMesh<T> defMesh(triangleSet, 0, xDirection, margin, Dot3D(0, 0, 0));
    defMesh.setDx((T) 1.);

    pcout << "Valor do Dx: " << defMesh.getDx() << std::endl;
    Array<T, 3> physical_position = defMesh.getPhysicalLocation();
    pcout << "posicao fisica em x: " << physical_position[0] << std::endl;
    pcout << "posicao fisica em y: " << physical_position[1] << std::endl;
    pcout << "posicao fisica em z: " << physical_position[2] << std::endl;
    //Array< T, 3 > position_lattice(50, ny/2, nz/2);
    //defMesh.setPhysicalLocation(position_lattice);
    Array<T, 3> physical_position_c = defMesh.getPhysicalLocation();
    pcout << "posicao fisica em x_c: " << physical_position_c[0] << std::endl;
    pcout << "posicao fisica em y_c: " << physical_position_c[1] << std::endl;
    pcout << "posicao fisica em z_c: " << physical_position_c[2] << std::endl;
    defMesh.getMesh().inflate();
    TriangleBoundary3D<T> boundary(defMesh);

    boundary.getMesh().writeBinarySTL("duct.stl");
    pcout << "Number of triangles: " << boundary.getMesh().getNumTriangles() << std::endl;
    //   handled by the following voxelization process.
    pcout << std::endl << "Voxelizing the domain." << std::endl;
    const int flowType = voxelFlag::outside;
    const plint extendedEnvelopeWidth = 2;
    const plint blockSize = 0; // Zero means: no sparse representation.
     // Because the Guo boundary condition needs 2-cell neighbor access.
    Box3D full_domain(0, nx-1, 0, ny-1, 0, nz-1);
    VoxelizedDomain3D<T> voxelizedDomain (
            boundary, flowType, full_domain, borderWidth, extendedEnvelopeWidth, blockSize);
    pcout << getMultiBlockInfo(voxelizedDomain.getVoxelMatrix()) << std::endl;

    // Build lattice and set default dynamics
    //-------------------------------------
    MultiBlockLattice3D<T,DESCRIPTOR> *lattice = 
    new MultiBlockLattice3D<T,DESCRIPTOR>(voxelizedDomain.getVoxelMatrix());

    // Setting anechoic dynamics like this way
    defineDynamics(*lattice, lattice->getBoundingBox(),
                new AnechoicBackgroundDynamics(omega));
    defineDynamics(*lattice, lattice->getBoundingBox(),
                new BackgroundDynamics(omega));
    //-------------------------------------


    //Set geometry in lattice
     // The Guo off *lattice boundary condition is set up.
    pcout << "Creating boundary condition." << std::endl;
    BoundaryProfiles3D<T,Velocity> profiles;
    //profiles.setWallProfile(new NoSlipProfile3D<T>);
    
    bool useAllDirections = true; // Extrapolation scheme for the off *lattice boundary condition.
    GuoOffLatticeModel3D<T,DESCRIPTOR>* model =
            new GuoOffLatticeModel3D<T,DESCRIPTOR> (
                new TriangleFlowShape3D<T,Array<T,3> > (
                    voxelizedDomain.getBoundary(), profiles),
                flowType, useAllDirections );
    bool useRegularized = true;
    // Use an off *lattice boundary condition which is closer in spirit to
    //   regularized boundary conditions.
    model->selectUseRegularizedModel(useRegularized);
    // ---
    OffLatticeBoundaryCondition3D<T,DESCRIPTOR,Velocity> boundaryCondition (
            model, voxelizedDomain, *lattice);
    boundaryCondition.insert();
    defineDynamics(*lattice, voxelizedDomain.getVoxelMatrix(),
     lattice->getBoundingBox(), new NoDynamics<T,DESCRIPTOR>(-999), voxelFlag::inside);

    pcout << "Creation of the lattice." << endl;

    // Switch off periodicity.
    lattice->periodicity().toggleAll(false);

    pcout << "Initilization of rho and u." << endl;
    initializeAtEquilibrium( *lattice, lattice->getBoundingBox(), rho0 , u0 );

    // Set anechoic condition on boundaries
    T rhoBar_target = 0;
    const T mach_number = 0.2;
    const T velocity_flow = mach_number*lattice_speed_sound;
    Array<T,3> j_target(0, 0, 0);
    T size_anechoic_buffer = 30;
    defineAnechoicMRTBoards(nx, ny, nz, *lattice, size_anechoic_buffer,
      omega, j_target, j_target, j_target, j_target, j_target, j_target,
      rhoBar_target);

    lattice->initialize();

    pcout << std::endl << "Voxelizing the domain." << std::endl;

    pcout << "Simulation begins" << endl;

    // Setting probes
    plint position_x_3r = centerLB[0] + size_duct - 3*radius;
    Box3D surface_probe_3r(position_x_3r, position_x_3r, ny/2 - radius/sqrt(2), 
            ny/2 + radius/sqrt(2), 
            nz/2 - radius/sqrt(2), 
            nz/2 + radius/sqrt(2));

    plint position_x_4r = centerLB[0] + size_duct - 4*radius;
    Box3D surface_probe_4r(position_x_4r, position_x_4r, ny/2 - radius/sqrt(2), 
            ny/2 + radius/sqrt(2), 
            nz/2 - radius/sqrt(2), 
            nz/2 + radius/sqrt(2));

    plint position_x_6r = centerLB[0] + size_duct - 0*radius;
    Box3D surface_probe_6r(position_x_6r, position_x_6r, ny/2 - radius/sqrt(2), 
            ny/2 + radius/sqrt(2), 
            nz/2 - radius/sqrt(2), 
            nz/2 + radius/sqrt(2));
    plb_ofstream history_pressures_3r("tmp/history_pressures_3r.dat");
    plb_ofstream history_pressures_4r("tmp/history_pressures_4r.dat");
    plb_ofstream history_pressures_6r("tmp/history_pressures_6r.dat");
    plb_ofstream history_velocities_3r("tmp/history_velocities_3r.dat");
    plb_ofstream history_velocities_4r("tmp/history_velocities_4r.dat");
    plb_ofstream history_velocities_6r("tmp/history_velocities_boca.dat");
    for (plint iT=0; iT<maxT; ++iT){
        
          if (iT <= maxT_final_source){
            //drho*sin(2*M_PI*(lattice_speed_sound/20)*iT);
            //drho*cos((lattice_speed_sound/radius)*(ka_max*((maxT-iT)/maxT)));
            
            T initial_frequency = ka_min*lattice_speed_sound/(2*M_PI*radius);
            T frequency_max_lattice = ka_max*lattice_speed_sound/(2*M_PI*radius);
            T variation_frequency = (frequency_max_lattice - initial_frequency)/maxT_final_source;
            T frequency_function = initial_frequency*iT + (variation_frequency*iT*iT)/2;
            T phase = 2*M_PI*frequency_function;
            T chirp_hand = 1. + drho*sin(phase);

            T rho_changing = 1. + drho*sin(2*M_PI*(lattice_speed_sound/20)*iT);
            //Box3D impulse(nx/2 + 50, nx/2 + 50, ny/2 + 50, ny/2 + 50, nz/2 + 50, nz/2 + 50);
            plint source_radius = radius;
             Box3D test_source(centerLB[0] + 10, centerLB[0] + 15, 
                ny/2 - source_radius/sqrt(2), 
                ny/2 + source_radius/sqrt(2), 
                nz/2 - source_radius/sqrt(2), 
                nz/2 + source_radius/sqrt(2));
            //Box3D impulse(centerLB[0] + 10, centerLB[0] + 10, ny/2, ny/2, nz/2, nz/2);
            initializeAtEquilibrium( *lattice, test_source, chirp_hand, u0);
        }else{
            plint source_radius = radius;
             Box3D test_source(centerLB[0] + 10, centerLB[0] + 15, 
                ny/2 - source_radius/sqrt(2), 
                ny/2 + source_radius/sqrt(2), 
                nz/2 - source_radius/sqrt(2), 
                nz/2 + source_radius/sqrt(2));
            initializeAtEquilibrium( *lattice, test_source, rho0, u0);
        }

        if (iT % 100 == 0 && iT>0) {
            pcout << "Iteration " << iT << endl;
            //writeGifs(lattice,iT);
            //writeVTK(*lattice, iT);
        }

        // extract values of pressure and velocities
        history_pressures_3r << setprecision(10) << (computeAverageDensity(*lattice, surface_probe_3r) - rho0)*cs2 << endl;
        history_pressures_4r << setprecision(10) << (computeAverageDensity(*lattice, surface_probe_4r) - rho0)*cs2 << endl;
        history_pressures_6r << setprecision(10) << (computeAverageDensity(*lattice, surface_probe_6r) - rho0)*cs2 << endl;

        history_velocities_3r << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe_3r, 0)/lattice_speed_sound << endl;

        history_velocities_4r << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe_4r, 1)/lattice_speed_sound << endl;

        history_velocities_6r << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe_6r, 2)/lattice_speed_sound << endl;

        lattice->collideAndStream();

    }

    pcout << "End of simulation at iteration " << endl;
}
