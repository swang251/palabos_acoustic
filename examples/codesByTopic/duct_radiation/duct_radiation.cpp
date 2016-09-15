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

    const plint nx = 350;
    const plint ny = 160;
    const plint nz = 160;
    const T lattice_speed_sound = 1/sqrt(3);
    const T cs2 = lattice_speed_sound*lattice_speed_sound;

    const T omega = 1.985;
    const plint maxT = 120000;

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
    T radius = 20;
    triangleSet = constructDuct(centerLB, radius + thickness_duct, radius
    , (T) 250, (plint) 10, (plint) 50);

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

    plb_ofstream history_pressures("tmp/history_pressures.dat");
    plb_ofstream history_velocities_x("tmp/history_velocities_x.dat");
    plb_ofstream history_velocities_y("tmp/history_velocities_y.dat");
    plb_ofstream history_velocities_z("tmp/history_velocities_z.dat");
    for (plint iT=0; iT<maxT; ++iT){
        
        if (iT != 0){
            T lattice_speed_sound = 1/sqrt(3);
            T rho_changing = 1. + drho*sin(2*M_PI*(lattice_speed_sound/20)*iT);

            // Set source sound
            T rhoBar_target_source = rho_changing - 1;
            plint source_radius = radius;
            AnechoicBackgroundDynamics *anechoicDynamics = 
            new AnechoicBackgroundDynamics(omega);
            T delta_efective = 30;
            anechoicDynamics->setDelta(delta_efective);
            anechoicDynamics->setRhoBar_target(rhoBar_target_source);
            anechoicDynamics->setJ_target(j_target);
            Box3D test_source(centerLB[0] + 10, centerLB[0] + 40, 
                ny/2 - source_radius/sqrt(2), 
                ny/2 + source_radius/sqrt(2), 
                nz/2 - source_radius/sqrt(2), 
                nz/2 + source_radius/sqrt(2));
            defineDynamics(*lattice, test_source, anechoicDynamics);

            //Box3D impulse(nx/2 + 50, nx/2 + 50, ny/2 + 50, ny/2 + 50, nz/2 + 50, nz/2 + 50);
            //Box3D impulse(centerLB[0] + 70, centerLB[0] + 70, ny/2, ny/2, nz/2, nz/2);
            //initializeAtEquilibrium( *lattice, impulse, rho_changing, u0);
        }

        if (iT % 10 == 0 && iT>0) {
            pcout << "Iteration " << iT << endl;
            //writeGifs(lattice,iT);
            //writeVTK(*lattice, iT);
        }

        // extract values of pressure and velocities
        Box3D surface_probe(6*radius, 6*radius, ny/2 - radius/sqrt(2), 
                ny/2 + radius/sqrt(2), 
                nz/2 - radius/sqrt(2), 
                nz/2 + radius/sqrt(2));

        history_pressures << setprecision(10) << (computeAverageDensity(*lattice, surface_probe) - rho0)*cs2 << endl;

        history_velocities_x << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe, 0)/lattice_speed_sound << endl;

        history_velocities_y << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe, 1)/lattice_speed_sound << endl;

        history_velocities_z << setprecision(10) << 
        boundaryCondition.computeAverageVelocityComponent(surface_probe, 2)/lattice_speed_sound << endl;

        lattice->collideAndStream();

    }

    pcout << "End of simulation at iteration " << endl;
}
