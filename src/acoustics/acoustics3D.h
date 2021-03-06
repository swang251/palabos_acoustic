
#include "palabos3D.h"
#ifndef PLB_PRECOMPILED // Unless precompiled version is used,
#include "palabos3D.hh"   // include full template code
#endif
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace plb;
using namespace std;

namespace plb_acoustics_3D{

	// Defining Matrix and Row
	typedef vector<T> Row;
	typedef vector< Row > Matrix;
	
	template<typename T, template<typename U> class Descriptor>
	void defineAnechoicWall(plint nx, plint ny,
	 MultiBlockLattice2D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, plint orientation, T omega, 
	  Array<plint, 2> position_anechoic_wall, plint length_anechoic_wall,
	  T rhoBar_target, Array<T,2> j_target){
		
		// delta increase to the right
		if(orientation == 1){
			for(T delta = 0; delta <= size_anechoic_buffer; delta++){        
		        DotList2D points_to_aplly_dynamics;
		        for (int i = 0; i <= length_anechoic_wall; ++i){
		            points_to_aplly_dynamics.addDot(
		            	Dot2D(position_anechoic_wall[0] + delta,
		            	position_anechoic_wall[1] + i));
		        }
		        AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
		        new AnechoicDynamics<T,DESCRIPTOR>(omega);
		        anechoicDynamics->setDelta((T) delta);
		        anechoicDynamics->setRhoBar_target(rhoBar_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        //j_target[0] = -j_target[0];  
		        anechoicDynamics->setJ_target(j_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
	    	}
		}

		// delta increase to the top
		else if(orientation == 4){
			for(T delta = 0; delta <= size_anechoic_buffer; delta++){        
		        DotList2D points_to_aplly_dynamics;
		        for (int i = 0; i <= length_anechoic_wall; ++i){
		            points_to_aplly_dynamics.addDot(
		            	Dot2D(position_anechoic_wall[0] + i,
		            	position_anechoic_wall[1] + delta));
		        }
		        AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
		        new AnechoicDynamics<T,DESCRIPTOR>(omega);
		        anechoicDynamics->setDelta((T) delta);
		        anechoicDynamics->setRhoBar_target(rhoBar_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        anechoicDynamics->setJ_target(j_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
	    	}
		}

		// delta increase to the left
		else if(orientation == 3){
			for(T delta = 0; delta <= size_anechoic_buffer; delta++){        
		        DotList2D points_to_aplly_dynamics;
		        for (int i = 0; i <= length_anechoic_wall; ++i){
		            points_to_aplly_dynamics.addDot(
		            	Dot2D(position_anechoic_wall[0] + delta,
		            	position_anechoic_wall[1] + i));
		        }
		        AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
		        new AnechoicDynamics<T,DESCRIPTOR>(omega);
		        T delta_left = size_anechoic_buffer - delta;
		        anechoicDynamics->setDelta(delta_left);
		        anechoicDynamics->setRhoBar_target(rhoBar_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        anechoicDynamics->setJ_target(j_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
	    	}
		}

		// delta increase to the bottom
		else if(orientation == 2){
			for(T delta = 0; delta <= size_anechoic_buffer; delta++){        
		        DotList2D points_to_aplly_dynamics;
		        for (int i = 0; i <= length_anechoic_wall; ++i){
		            points_to_aplly_dynamics.addDot(
		            	Dot2D(position_anechoic_wall[0] + i,
		            	position_anechoic_wall[1] +  delta));
		        }
		        AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
		        new AnechoicDynamics<T,DESCRIPTOR>(omega);
		        T delta_left = size_anechoic_buffer - delta;
		        anechoicDynamics->setDelta(delta_left);
		        anechoicDynamics->setRhoBar_target(rhoBar_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        anechoicDynamics->setJ_target(j_target);
		        anechoicDynamics->setBuffer_size(size_anechoic_buffer);
		        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
	    	}
		}

		else{
			cout << "Anechoic Dynamics not Defined." << endl;
			cout << "Choose the correct orientation number." << endl;
		}

	}

	template<typename T, template<typename U> class Descriptor>
	void defineAnechoicBoards(plint nx, plint ny, plint nz,
	 MultiBlockLattice3D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, T omega, 
	  Array<T,3> j_target_normal_z_positive,
	  Array<T,3> j_target_normal_z_negative,
	  Array<T,3> j_target_normal_y_positive,
	  Array<T,3> j_target_normal_y_negative,
	  Array<T,3> j_target_normal_x_positive,
	  Array<T,3> j_target_normal_x_negative,
	  T rhoBar_target){
	  	/* if size_anechoic_buffer is not equal size_anechoic_buffer, we have to change 
	  	total_distance in file dynamicsTemplates3D.h too*/
	 	for(T delta = 0; delta <= size_anechoic_buffer; delta++){
			// for in all points-cell lattice
	 		for(plint z = 0; z < nz; z++){
				for(plint y = 0; y < ny; y++){
					for(plint x = 0; x < nx; x++){
						// condition to right (1)
						if(x == (nx-delta) &&
						  y >= (delta-1) && 
						  y < (ny-delta) &&
						  z >= (delta-1) && 
						  z < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_x_positive);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
						// condition to bottom (2)
						else if(x >= (delta-1) &&
						  x < (nx-delta) &&
						  y == delta &&
						  z >= (delta-1) && 
						  z < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_y_negative);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
						// condition to left (3)
						else if(x == delta &&
						  y >= (delta-1) &&
						  y < (ny-delta) &&
						  z >= (delta-1) && 
						  z < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_x_negative);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
						// condition to top (4)
						else if(x >= (delta-1) &&
						  x < (nx-delta) &&
						  y == (ny-delta) &&
						  z >= (delta-1) && 
						  z < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_y_positive);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
						// condition to front (5)
						else if(x >= (delta-1) &&
						  x < (nx-delta) &&
						  z == (nz-delta) &&
						  y >= (delta-1) && 
						  y < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_z_positive);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
						// condition to back (6)
						else if(x >= (delta-1) &&
						  x < (nx-delta) &&
						  z == delta &&
						  y >= (delta-1) && 
						  y < (nz-delta)){
							// set delta here
							AnechoicDynamics<T,DESCRIPTOR> *anechoicDynamics = 
							new AnechoicDynamics<T,DESCRIPTOR>(omega);
							T delta_efective = size_anechoic_buffer - delta;
							anechoicDynamics->setDelta(delta_efective);
							anechoicDynamics->setRhoBar_target(rhoBar_target);
							anechoicDynamics->setBuffer_size(size_anechoic_buffer);
							anechoicDynamics->setJ_target(j_target_normal_z_negative);
							DotList3D points_to_aplly_dynamics;
							points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
							defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
						}
					}
				}
			}
		}
	}

	template<typename T, template<typename U> class Descriptor>
	void defineAnechoicMRTBoards(plint nx, plint ny, plint nz,
	 MultiBlockLattice3D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, T omega, 
	  Array<T,3> j_target_normal_z_positive,
	  Array<T,3> j_target_normal_z_negative,
	  Array<T,3> j_target_normal_y_positive,
	  Array<T,3> j_target_normal_y_negative,
	  Array<T,3> j_target_normal_x_positive,
	  Array<T,3> j_target_normal_x_negative,
	  T rhoBar_target){

		j_target_normal_z_positive = -j_target_normal_z_positive;
	  	j_target_normal_z_negative = -j_target_normal_z_negative;
	  	j_target_normal_y_positive = -j_target_normal_y_positive;
	  	j_target_normal_y_negative = -j_target_normal_y_negative;
	  	j_target_normal_x_positive = -j_target_normal_x_positive;
	  	j_target_normal_x_negative = -j_target_normal_x_negative;

	  	typedef AnechoicMRTdynamics<T,DESCRIPTOR> AnechoicBackgroundDynamics;
	  	/* if size_anechoic_buffer is not equal size_anechoic_buffer, we have to change 
	  	total_distance in file dynamicsTemplates3D.h too*/
	 	for(T delta = 0; delta <= size_anechoic_buffer; delta++){
			// for in all points-cell lattice
	 		for(plint z = 0; z < nz; z++){
				for(plint y = 0; y < ny; y++){
					for(plint x = 0; x < nx; x++){
						// condition to right (1)
                        if(x == (nx-delta) &&
                          y >= (delta-1) && 
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_x_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to bottom (2)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == delta &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_y_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to left (3)
                        else if(x == delta &&
                          y >= (delta-1) &&
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_x_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to top (4)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_y_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to front (5)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == (nz-delta) &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_z_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to back (6)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == delta &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_z_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
					}
				}
			}
		}
	}


/*defineAnechoicMRTWall(plint nx, plint ny, plint nz,
	 MultiBlockLattice3D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, plint orientation, T omega, 
	  Array<plint, 3> position_anechoic_wall, plint length_anechoic_wall,plint width_anechoic_wall,
	  T rhoBar_target, Array<T,3> j_target)*/

template<typename T, template<typename U> class Descriptor>
	void defineAnechoicMRTWall(plint nx, plint ny, plint nz,
	 MultiBlockLattice3D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, T omega, 
	  Array<T,3> j_target,
	  T rhoBar_target, plint orientation){

		j_target = -j_target;

	  	typedef AnechoicMRTdynamics<T,DESCRIPTOR> AnechoicBackgroundDynamics;

	 	for(T delta = 0; delta <= size_anechoic_buffer; delta++){
			// for in all points-cell lattice
	 		for(plint z = 0; z < nz; z++){
				for(plint y = 0; y < ny; y++){
					for(plint x = 0; x < nx; x++){
						// condition to right (1)
                        if(x == (nx-delta) &&
                          y >= (delta-1) && 
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            //defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to bottom (2)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == delta &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            //defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to left (3)
                        else if(x == delta &&
                          y >= (delta-1) &&
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            //defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to top (4)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            //defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to front (5)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == (nz-delta) &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to back (6)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == delta &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            //defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
					}
				}
			}
		}
	}


template<typename T, template<typename U> class Descriptor>
	void defineAnechoicMRTBoards_limited(plint nx, plint ny, plint nz,
	 MultiBlockLattice3D<T,Descriptor>& lattice,
	  T size_anechoic_buffer, T omega, 
	  Array<T,3> j_target_normal_z_positive,
	  Array<T,3> j_target_normal_z_negative,
	  Array<T,3> j_target_normal_y_positive,
	  Array<T,3> j_target_normal_y_negative,
	  Array<T,3> j_target_normal_x_positive,
	  Array<T,3> j_target_normal_x_negative,
	  T rhoBar_target, plint off_set_z){

		j_target_normal_z_positive = -j_target_normal_z_positive;
	  	j_target_normal_z_negative = -j_target_normal_z_negative;
	  	j_target_normal_y_positive = -j_target_normal_y_positive;
	  	j_target_normal_y_negative = -j_target_normal_y_negative;
	  	j_target_normal_x_positive = -j_target_normal_x_positive;
	  	j_target_normal_x_negative = -j_target_normal_x_negative;

	  	typedef AnechoicMRTdynamics<T,DESCRIPTOR> AnechoicBackgroundDynamics;
	  	/* if size_anechoic_buffer is not equal size_anechoic_buffer, we have to change 
	  	total_distance in file dynamicsTemplates3D.h too*/
	 	for(T delta = 0; delta <= size_anechoic_buffer; delta++){
			// for in all points-cell lattice
	 		for(plint z = off_set_z; z < nz; z++){
				for(plint y = 0; y < ny; y++){
					for(plint x = 0; x < nx; x++){
						// condition to right (1)
                        if(x == (nx-delta) &&
                          y >= (delta-1) && 
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_x_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to bottom (2)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == delta &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_y_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to left (3)
                        else if(x == delta &&
                          y >= (delta-1) &&
                          y < (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_x_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to top (4)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          y == (ny-delta) &&
                          z >= (delta-1) && 
                          z < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_y_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to front (5)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == (nz-delta) &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_z_positive);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
                        // condition to back (6)
                        else if(x >= (delta-1) &&
                          x < (nx-delta) &&
                          z == delta + off_set_z &&
                          y >= (delta-1) && 
                          y < (nz-delta)){
                            // set delta here
                            AnechoicBackgroundDynamics *anechoicDynamics = 
                            new AnechoicBackgroundDynamics(omega);
                            T delta_efective = size_anechoic_buffer - delta;
                            anechoicDynamics->setDelta(delta_efective);
                            anechoicDynamics->setRhoBar_target(rhoBar_target);
                            anechoicDynamics->setBuffer_size(size_anechoic_buffer);
                            anechoicDynamics->setJ_target(j_target_normal_z_negative);
                            DotList3D points_to_aplly_dynamics;
                            points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                            defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        }
					}
				}
			}
		}
	}


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void writeGifs(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint iter, T rho0, T drho){
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

void writeVTK(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint iter, T rho0, T drho, Box3D local_to_extract){
        VtkImageOutput3D<T> vtkOut(createFileName("vtk", iter, 6), 1.);
        vtkOut.writeData<float>(*computeDensity(lattice, local_to_extract), "density", 1.);
        std::auto_ptr<MultiScalarField3D<T> > velocity(plb::computeVelocityComponent(lattice, local_to_extract, 2));
        
        vtkOut.writeData<T>(*velocity, "velocity", 1.);
}

void build_duct(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint nx, plint ny,
    Array<plint,3> position, plint radius, plint length, plint thickness, T omega){
    length += 4;
    T mach_number = 0;
    T lattice_speed_sound = 1/sqrt(3);
    T velocity_flow = -mach_number*lattice_speed_sound;
    plint anechoic_size = 29;
    // Duct is constructed along the Z direction
    //plint size_square = 50;
    plint size_square = 2*radius;
    plint radius_intern = radius - thickness;
    for (plint x = position[0] - radius; x < nx/2 + size_square/2; ++x){
        for (plint y = position[1] - radius; y < ny/2 + size_square/2; ++y){
            for (plint z = position[2]; z < length + position[2]; ++z){

                if (radius*radius > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2)){
                    DotList3D points_to_aplly_dynamics;
                    points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                    defineDynamics(lattice, points_to_aplly_dynamics, new BounceBack<T,DESCRIPTOR>(0));
                }
                // extrude
                if (radius_intern*radius_intern > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2) && z > position[2] + 2){
                    DotList3D points_to_aplly_dynamics;
                    points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                    if (z < position[2] + 2 + anechoic_size && z > position[2] + 3){
                        Array<T,3> u0(0, 0, velocity_flow);
                        T rhoBar_target = 0;
                        AnechoicBackgroundDynamics *anechoicDynamics = 
                        new AnechoicBackgroundDynamics(omega);
                        T delta_efective = anechoic_size - z - position[2] - 2;
                        anechoicDynamics->setDelta(delta_efective);
                        anechoicDynamics->setRhoBar_target(rhoBar_target);
                        anechoicDynamics->setBuffer_size(anechoic_size);
                        anechoicDynamics->setJ_target(u0);
                        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        
                    }else{
                        defineDynamics(lattice, points_to_aplly_dynamics, new BackgroundDynamics(omega));
                    }
                }
            }
        }
    }
}

T get_linear_chirp(T ka_min, T ka_max, plint maxT_final_source, plint iT, T drho, T radius){
    T lattice_speed_sound = 1/sqrt(3);
    T initial_frequency = ka_min*lattice_speed_sound/(2*M_PI*radius);
    T frequency_max_lattice = ka_max*lattice_speed_sound/(2*M_PI*radius);
    T variation_frequency = (frequency_max_lattice - initial_frequency)/maxT_final_source;
    T frequency_function = initial_frequency*iT + (variation_frequency*iT*iT)/2;
    T phase = 2*M_PI*frequency_function;
    T chirp_hand = 1. + drho*sin(phase);

    return chirp_hand;
}

T get_linear_chirp_AZ(T ka_max, plint total_signals, plint maxT_final_source, plint iT, T drho, T radius){
    T cs = 1/sqrt(3);
    T chirp_value = 1;
    total_signals = 2*total_signals;
    for (plint n_signal = 1; n_signal <= total_signals; n_signal++){
        T interval = ka_max/total_signals;
        T phase = (n_signal*interval*cs*iT)/(radius);
        chirp_value += drho*sin(phase);
    }
    return chirp_value;
}

T get_tonal(T ka, plint maxT_final_source, plint iT, T drho, T radius){
	T cs = 1/sqrt(3);
    T chirp_value = 1;
    T phase = (ka*cs*iT)/(radius);
    chirp_value += drho*sin(phase);
	return chirp_value;
}

T get_linear_chirp_AZ_freq_omega(T freq_omega_max, plint total_signals, plint maxT_final_source, plint iT, T drho){
    T cs = 1/sqrt(3);
    T chirp_value = 1;
    total_signals = 2*total_signals;
    for (plint n_signal = 1; n_signal <= total_signals; n_signal++){
        T interval = freq_omega_max/total_signals;
        T phase = (n_signal*interval*iT);
        chirp_value += drho*sin(phase);
    }
    return chirp_value;
}

void set_source(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, Array<plint,3> position, 
    T chirp_hand, Array<T,3> u0, plint radius, plint radius_intern, plint nx, plint ny){

    plint size_square = 2*radius;
    Box3D impulse_local;
    for (plint x = position[0] - radius; x < nx/2 + size_square/2; ++x){
        for (plint y = position[1] - radius; y < ny/2 + size_square/2; ++y){
            // extrude
            if (radius_intern*radius_intern > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2)){
                Array<plint, 6> local_source_2(x, x, y, y, position[2] + 29, position[2] + 30);
                impulse_local.from_plbArray(local_source_2);
                Array<T,3> u_chirp_hand(0, 0, u0[2] + (chirp_hand-1)*(sqrt(3)));
                initializeAtEquilibrium(lattice, impulse_local, chirp_hand, u_chirp_hand);
            }
        }
    }
}

T compute_avarage_density(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, Array<plint,3> position, 
    plint radius, plint radius_intern, plint nx, plint ny, plint position_z){

    plint number_cells = 0;
    T average_density = 0;
    plint size_square = 2*radius;
    Box3D local_box;
    for (plint x = position[0] - radius; x < nx/2 + size_square/2; ++x){
        for (plint y = position[1] - radius; y < ny/2 + size_square/2; ++y){
            // extrude
            if (radius_intern*radius_intern > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2)){
                Array<plint, 6> local(x, x, y, y, position_z, position_z);
                local_box.from_plbArray(local);
                average_density += computeAverageDensity(lattice, local_box);
                number_cells += 1;
            }
        }
    }

    average_density = average_density/number_cells;

    return average_density;
}

T compute_avarage_velocity(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, Array<plint,3> position, 
    plint radius, plint radius_intern, plint nx, plint ny, plint position_z){

    plint number_cells = 0;
    T average_velocity = 0;
    plint size_square = 2*radius;
    Box3D local_box;
    for (plint x = position[0] - radius; x < nx/2 + size_square/2; ++x){
        for (plint y = position[1] - radius; y < ny/2 + size_square/2; ++y){
            // extrude
            if (radius_intern*radius_intern > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2)){
                Array<plint, 6> local(x, x, y, y, position_z, position_z);
                local_box.from_plbArray(local);
                std::auto_ptr<MultiScalarField3D<T> > velocity(plb::computeVelocityComponent(lattice, local_box, 2));
                average_velocity += computeAverage(*velocity, local_box);
                number_cells += 1;
            }
        }
    }

    average_velocity = average_velocity/number_cells;

    return average_velocity;
}


void set_nodynamics(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint nx, plint ny, plint off_set_z){
    Box3D place_nodynamics(0, nx - 1, 0, ny - 1, 0, off_set_z);
    defineDynamics(lattice, place_nodynamics, new NoDynamics<T,DESCRIPTOR>(0));
}

class Probe{
    public:
        Box3D location;
        string name_probe;
        plb_ofstream file_pressures;
        plb_ofstream file_velocities_x;
        plb_ofstream file_velocities_y;
        plb_ofstream file_velocities_z;
    public:
        Probe() : location(), name_probe(), file_pressures(), file_velocities_x(),
        file_velocities_y(), file_velocities_z(){ }
        Probe(Box3D location, string directory, string name_probe){

            directory = directory + "/" + name_probe;
            std::string command = "mkdir -p " + directory;
            char to_char_command[1024];
            strcpy(to_char_command, command.c_str());
            system(to_char_command);

            string pressures_string = directory + "/history_pressures_" + name_probe + ".dat";
            string velocities_x_string = directory + "/history_velocities_x_" + name_probe + ".dat";
            string velocities_y_string = directory + "/history_velocities_y_" + name_probe + ".dat";
            string velocities_z_string = directory + "/history_velocities_z_" + name_probe + ".dat";
            char to_char_pressures[1024];
            char to_char_velocities_x[1024];
            char to_char_velocities_y[1024];
            char to_char_velocities_z[1024];
            strcpy(to_char_pressures, pressures_string.c_str());
            strcpy(to_char_velocities_x, velocities_x_string.c_str());
            strcpy(to_char_velocities_y, velocities_y_string.c_str());
            strcpy(to_char_velocities_z, velocities_z_string.c_str());

            this->file_pressures.open(to_char_pressures);
            this->file_velocities_x.open(to_char_velocities_x);
            this->file_velocities_y.open(to_char_velocities_y);
            this->file_velocities_z.open(to_char_velocities_z);
            this->location = location;
            this->name_probe = name_probe;
        }

        string get_name_probe(){
            return this->name_probe;
        }

        void set_properties(Box3D location, string directory, string name_probe){
            directory = directory + "/" + name_probe;
            std::string command = "mkdir -p " + directory;
            char to_char_command[1024];
            strcpy(to_char_command, command.c_str());
            system(to_char_command);

            string pressures_string = directory + "/history_pressures_" + name_probe + ".dat";
            string velocities_x_string = directory + "/history_velocities_x_" + name_probe + ".dat";
            string velocities_y_string = directory + "/history_velocities_y_" + name_probe + ".dat";
            string velocities_z_string = directory + "/history_velocities_z_" + name_probe + ".dat";
            char to_char_pressures[1024];
            char to_char_velocities_x[1024];
            char to_char_velocities_y[1024];
            char to_char_velocities_z[1024];
            strcpy(to_char_pressures, pressures_string.c_str());
            strcpy(to_char_velocities_x, velocities_x_string.c_str());
            strcpy(to_char_velocities_y, velocities_y_string.c_str());
            strcpy(to_char_velocities_z, velocities_z_string.c_str());

            this->file_pressures.open(to_char_pressures);
            this->file_velocities_x.open(to_char_velocities_x);
            this->file_velocities_y.open(to_char_velocities_y);
            this->file_velocities_z.open(to_char_velocities_z);
            this->location = location;
            this->name_probe = name_probe;   
        }

        void save_point(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, T rho0, T cs2){
            this->file_pressures << setprecision(10) << (computeAverageDensity(lattice, this->location) - rho0)*cs2 << endl;
            std::auto_ptr<MultiScalarField3D<T> > velocity_x(plb::computeVelocityComponent(lattice, this->location, 0));
            this->file_velocities_x << setprecision(10) << computeAverage(*velocity_x, this->location) << endl;
            std::auto_ptr<MultiScalarField3D<T> > velocity_y(plb::computeVelocityComponent(lattice, this->location, 1));
            this->file_velocities_y << setprecision(10) << computeAverage(*velocity_y, this->location) << endl;
            std::auto_ptr<MultiScalarField3D<T> > velocity_z(plb::computeVelocityComponent(lattice, this->location, 2));
            this->file_velocities_z << setprecision(10) << computeAverage(*velocity_z, this->location) << endl;
        }
};

class Two_Microphones{
    private:
        Probe microphone_1;
        Probe microphone_2;
    public:
        Two_Microphones(plint radius, plint microphone_distance, 
            plint length_duct, Array<plint,3> position_duct, 
            std::string fNameOut, std::string name, plint distance, 
            plint nx, plint ny, plint nz){

            plint radius_probe = (radius - 1)/sqrt(2);
            plint position_z = position_duct[2] + length_duct - distance;
            
            Box3D surface_probe_p1(nx/2 - (radius_probe)/sqrt(2), 
                    nx/2 + (radius_probe)/sqrt(2), 
                    ny/2 - (radius_probe)/sqrt(2), 
                    ny/2 + (radius_probe)/sqrt(2),
                    position_z, position_z);
            std::string name_p1 = name + "_p1";
            Probe probe_p1(surface_probe_p1, fNameOut, name_p1);
            this->microphone_1.set_properties(surface_probe_p1, fNameOut, name_p1);

            Box3D surface_probe_p2(nx/2 - (radius_probe)/sqrt(2), 
                    nx/2 + (radius_probe)/sqrt(2), 
                    ny/2 - (radius_probe)/sqrt(2), 
                    ny/2 + (radius_probe)/sqrt(2),
                    position_z + microphone_distance,
                    position_z + microphone_distance);
            std::string name_p2 = name + "_p2";
            this->microphone_2.set_properties(surface_probe_p2, fNameOut, name_p2);
        }

        void save_point(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, T rho0, T cs2){
            this->microphone_1.save_point(lattice, rho0, cs2);
            this->microphone_2.save_point(lattice, rho0, cs2);   
        }
};

class Coefficient_Reflection_Probes{
    private:
        Probe probe_A;
        Probe probe_B;
        Probe probe_C;
        Probe probe_D;
    public:
        Coefficient_Reflection_Probes(Array<plint,3> position_A,
        	Array<plint,3> position_B,
        	Array<plint,3> position_C,
        	Array<plint,3> position_D,
            std::string fNameOut, std::string name){

        	Box3D surface_probe_A(position_A[0], position_A[0], position_A[1], position_A[1], position_A[2], position_A[2]);
        	Box3D surface_probe_B(position_B[0], position_B[0], position_B[1], position_B[1], position_B[2], position_B[2]);
        	Box3D surface_probe_C(position_C[0], position_C[0], position_C[1], position_C[1], position_C[2], position_C[2]);
        	Box3D surface_probe_D(position_D[0], position_D[0], position_D[1], position_D[1], position_D[2], position_D[2]);

            std::string name_A = name + "_A";
            Probe probe_A(surface_probe_A, fNameOut, name_A);
            this->probe_A.set_properties(surface_probe_A, fNameOut, name_A);

            std::string name_B = name + "_B";
            Probe probe_B(surface_probe_B, fNameOut, name_B);
            this->probe_B.set_properties(surface_probe_B, fNameOut, name_B);

            std::string name_C = name + "_C";
            Probe probe_C(surface_probe_C, fNameOut, name_C);
            this->probe_C.set_properties(surface_probe_C, fNameOut, name_C);

            std::string name_D = name + "_D";
            Probe probe_D(surface_probe_D, fNameOut, name_D);
            this->probe_D.set_properties(surface_probe_D, fNameOut, name_D);
            
        }

        void save_point(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, T rho0, T cs2){
            this->probe_A.save_point(lattice, rho0, cs2);
            this->probe_B.save_point(lattice, rho0, cs2);
            this->probe_C.save_point(lattice, rho0, cs2);
            this->probe_D.save_point(lattice, rho0, cs2);
        }
};



class System_Abom_Measurement{
    private:
        std::vector<Box3D> microphones_positions; 
        plb_ofstream file_pressures;
        plb_ofstream file_velocities_x;
        plb_ofstream file_velocities_y;
        plb_ofstream file_velocities_z;
    public:
        System_Abom_Measurement(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, Array<plint,3> position_duct, 
            plint begin_microphone, plint length_duct, plint radius, string directory){

            string name_probe = "system_abom_measurement_data";
            directory = directory + "/" + name_probe;
            std::string command = "mkdir -p " + directory;
            char to_char_command[1024];
            strcpy(to_char_command, command.c_str());
            system(to_char_command);

            /*const plint nx = lattice.getNx();
            const plint ny = lattice.getNy();
            const plint nz = lattice.getNz();
            const plint diameter = 2*radius;*/

            string pressures_string = directory + "/history_pressures_" + name_probe + ".dat";
            string velocities_x_string = directory + "/history_velocities_x_" + name_probe + ".dat";
            string velocities_y_string = directory + "/history_velocities_y_" + name_probe + ".dat";
            string velocities_z_string = directory + "/history_velocities_z_" + name_probe + ".dat";
            char to_char_pressures[1024];
            char to_char_velocities_x[1024];
            char to_char_velocities_y[1024];
            char to_char_velocities_z[1024];
            strcpy(to_char_pressures, pressures_string.c_str());
            strcpy(to_char_velocities_x, velocities_x_string.c_str());
            strcpy(to_char_velocities_y, velocities_y_string.c_str());
            strcpy(to_char_velocities_z, velocities_z_string.c_str());

            this->file_pressures.open(to_char_pressures);
            this->file_velocities_x.open(to_char_velocities_x);
            this->file_velocities_y.open(to_char_velocities_y);
            this->file_velocities_z.open(to_char_velocities_z);

            plint radius_probe = (radius - 1)/sqrt(2);
            for (int i = begin_microphone; i <= length_duct + 3 + radius/2; i++){
            	plint position_microphone = position_duct[2] + i;

            	Box3D surface_microphone(
            		position_duct[0] - (radius_probe)/sqrt(2), 
            		position_duct[0] + (radius_probe)/sqrt(2),
                	position_duct[1] - (radius_probe)/sqrt(2), 
                	position_duct[1] + (radius_probe)/sqrt(2),
                	position_microphone, 
                	position_microphone);

            	this->microphones_positions.push_back(surface_microphone);
            }

            pcout << "MICROPHONES" << endl;
            for (int i = 0; i < this->microphones_positions.size(); i++){
                Box3D to_see = microphones_positions[i];
                Array<plint, 6> test = to_see.to_plbArray();
                pcout << i + 1 << " - " << test[5] << "; ";
            }
        }

    void save_point(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, T rho0, T cs2){
        for (int mic = 0; mic < this->microphones_positions.size(); mic++){
            file_pressures << setprecision(10) << (computeAverageDensity(lattice, this->microphones_positions[mic]) - rho0)*cs2 << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_x(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 0));
            file_velocities_x << setprecision(10) << computeAverage(*velocity_x, this->microphones_positions[mic]) << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_y(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 1));
            file_velocities_y << setprecision(10) << computeAverage(*velocity_y, this->microphones_positions[mic]) << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_z(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 2));
            file_velocities_z << setprecision(10) << computeAverage(*velocity_z, this->microphones_positions[mic]) << " ";
        }
        file_pressures << endl;
        file_velocities_x << endl;
        file_velocities_y << endl;
        file_velocities_z << endl;
    }
};


class System_Abom_Measurement_points{
    private:
        std::vector<Box3D> microphones_positions; 
        plb_ofstream file_pressures;
        plb_ofstream file_velocities_x;
        plb_ofstream file_velocities_y;
        plb_ofstream file_velocities_z;
    public:
        System_Abom_Measurement_points(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, Array<plint,3> position_duct, 
            plint begin_microphone, plint length_duct, plint radius, string directory, Array<plint,2> centering_point, string name_probe){

            directory = directory + "/" + name_probe;
            std::string command = "mkdir -p " + directory;
            char to_char_command[1024];
            strcpy(to_char_command, command.c_str());
            system(to_char_command);

            /*const plint nx = lattice.getNx();
            const plint ny = lattice.getNy();
            const plint nz = lattice.getNz();
            const plint diameter = 2*radius;*/

            string pressures_string = directory + "/history_pressures_" + name_probe + ".dat";
            string velocities_x_string = directory + "/history_velocities_x_" + name_probe + ".dat";
            string velocities_y_string = directory + "/history_velocities_y_" + name_probe + ".dat";
            string velocities_z_string = directory + "/history_velocities_z_" + name_probe + ".dat";
            char to_char_pressures[1024];
            char to_char_velocities_x[1024];
            char to_char_velocities_y[1024];
            char to_char_velocities_z[1024];
            strcpy(to_char_pressures, pressures_string.c_str());
            strcpy(to_char_velocities_x, velocities_x_string.c_str());
            strcpy(to_char_velocities_y, velocities_y_string.c_str());
            strcpy(to_char_velocities_z, velocities_z_string.c_str());

            this->file_pressures.open(to_char_pressures);
            this->file_velocities_x.open(to_char_velocities_x);
            this->file_velocities_y.open(to_char_velocities_y);
            this->file_velocities_z.open(to_char_velocities_z);

            plint radius_probe = (radius - 1)/sqrt(2);
            for (int i = begin_microphone; i <= length_duct + 3 + radius/2; i++){
            	plint position_microphone = position_duct[2] + i;

            	Box3D surface_microphone(
            		position_duct[0] + centering_point[0], 
            		position_duct[0] + centering_point[0],
                	position_duct[1] + centering_point[1], 
                	position_duct[1] + centering_point[1],
                	position_microphone, 
                	position_microphone);

            	this->microphones_positions.push_back(surface_microphone);
            }

            pcout << "MICROPHONES" << endl;
            for (int i = 0; i < this->microphones_positions.size(); i++){
                Box3D to_see = microphones_positions[i];
                Array<plint, 6> test = to_see.to_plbArray();
                pcout << i + 1 << " - " << test[5] << "; ";
            }
        }

    void save_point(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, T rho0, T cs2){
        for (int mic = 0; mic < this->microphones_positions.size(); mic++){
            file_pressures << setprecision(10) << (computeAverageDensity(lattice, this->microphones_positions[mic]) - rho0)*cs2 << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_x(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 0));
            file_velocities_x << setprecision(10) << computeAverage(*velocity_x, this->microphones_positions[mic]) << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_y(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 1));
            file_velocities_y << setprecision(10) << computeAverage(*velocity_y, this->microphones_positions[mic]) << " ";
            std::auto_ptr<MultiScalarField3D<T> > velocity_z(plb::computeVelocityComponent(lattice, this->microphones_positions[mic], 2));
            file_velocities_z << setprecision(10) << computeAverage(*velocity_z, this->microphones_positions[mic]) << " ";
        }
        file_pressures << endl;
        file_velocities_x << endl;
        file_velocities_y << endl;
        file_velocities_z << endl;
    }
};

void build_duct_with_horn(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, plint nx, plint ny,
    Array<plint,3> position, plint radius, plint length, plint thickness, T omega, plint size_horn){
    length += 4;
    T mach_number = 0;
    T lattice_speed_sound = 1/sqrt(3);
    T velocity_flow = -mach_number*lattice_speed_sound;
    plint anechoic_size = 29;
    // Duct is constructed along the Z direction
    //plint size_square = 50;
    plint size_square = 2*radius;
    plint radius_intern = radius - thickness;
    for (plint x = position[0] - radius; x < nx/2 + size_square/2; ++x){
        for (plint y = position[1] - radius; y < ny/2 + size_square/2; ++y){
            for (plint z = position[2]; z < length + position[2]; ++z){

                if (radius*radius > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2)){
                    DotList3D points_to_aplly_dynamics;
                    points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                    defineDynamics(lattice, points_to_aplly_dynamics, new BounceBack<T,DESCRIPTOR>(0));
                }
                // extrude
                if (radius_intern*radius_intern > (x-nx/2)*(x-nx/2) + (y-ny/2)*(y-ny/2) && z > position[2] + 2){
                    DotList3D points_to_aplly_dynamics;
                    points_to_aplly_dynamics.addDot(Dot3D(x,y,z));
                    if (z < position[2] + 2 + anechoic_size && z > position[2] + 3){
                        Array<T,3> u0(0, 0, velocity_flow);
                        T rhoBar_target = 0;
                        AnechoicBackgroundDynamics *anechoicDynamics = 
                        new AnechoicBackgroundDynamics(omega);
                        T delta_efective = anechoic_size - z - position[2] - 2;
                        anechoicDynamics->setDelta(delta_efective);
                        anechoicDynamics->setRhoBar_target(rhoBar_target);
                        anechoicDynamics->setBuffer_size(anechoic_size);
                        anechoicDynamics->setJ_target(u0);
                        defineDynamics(lattice, points_to_aplly_dynamics, anechoicDynamics);
                        
                    }else{
                        defineDynamics(lattice, points_to_aplly_dynamics, new BackgroundDynamics(omega));
                    }
                }
            }
        }
    }
}

T compute_drho(T NPS){
	T p_ref = 2*10e-5;
	T p_line_phis =  p_ref*pow(10, (NPS/20));
	T cs = 1/sqrt(3);
	T c_o = 343;
	T rho_o = 1.22;
	T qsi = c_o/cs;
	T p_line_lattice = p_line_phis/((qsi*qsi)*rho_o);
	T delta_rho = p_line_lattice/(cs*cs);
	return delta_rho;
}

class Howe_Corollary{
private:
    Box3D plane;
    std::vector<MultiScalarField3D<T> > several_mean_velocities_axial; // z
    std::vector<MultiScalarField3D<T> > several_mean_velocities_upright; // y or x because it's axyssimetric
    plint total_period;
    plint initial_time;
public:
	Howe_Corollary(Box3D plane, plint total_period, plint initial_time){
		this->plane = plane;
		this->total_period = total_period;
		this->initial_time = initial_time;
	}

	plint get_total_period(){
		return this->total_period;
	}

	plint get_initial_time(){
		return this->initial_time;
	}

	void extract_velocities(MultiBlockLattice3D<T,DESCRIPTOR>& lattice){
			 std::auto_ptr<MultiScalarField3D<T> > velocity_z(plb::computeVelocityComponent(lattice, this->plane, 2));
			 this->several_mean_velocities_axial.push_back(*velocity_z);
			 std::auto_ptr<MultiScalarField3D<T> > velocity_x(plb::computeVelocityComponent(lattice, this->plane, 0));
			 this->several_mean_velocities_upright.push_back(*velocity_x);
	}

	void calculate_acoustic_energy(string directory, string name_file){
		
        for (int i = 0; i < this->several_mean_velocities_axial.size(); i++){
        	plb_ofstream howe_corollary_result_axial;    
        	std::ostringstream ss;
     		ss << i;
        	string howe_string = directory + "/" + name_file + ss.str() + "_axial" + ".dat";
            char to_char_howe[1024];
	        strcpy(to_char_howe, howe_string.c_str());
	        howe_corollary_result_axial.open(to_char_howe);
	        howe_corollary_result_axial << setprecision(10) << (this->several_mean_velocities_axial[i]);
	        howe_corollary_result_axial.close();
	        /*VtkImageOutput3D<T> vtkOut(createFileName(name_file, i, 6), 1.);
        	vtkOut.writeData<T>((this->several_mean_velocities_axial[i]), "velocity", 1.);*/
        }


        for (int i = 0; i < this->several_mean_velocities_upright.size(); i++){
        	plb_ofstream howe_corollary_result_upright;    
        	std::ostringstream ss;
     		ss << i;
        	string howe_string = directory + "/" + name_file + ss.str() + "_upright" + ".dat";
            char to_char_howe[1024];
	        strcpy(to_char_howe, howe_string.c_str());
	        howe_corollary_result_upright.open(to_char_howe);
	        howe_corollary_result_upright << setprecision(10) << (this->several_mean_velocities_upright[i]);
	        howe_corollary_result_upright.close();
	        /*VtkImageOutput3D<T> vtkOut(createFileName(name_file, i, 6), 1.);
        	vtkOut.writeData<T>((this->several_mean_velocities_axial[i]), "velocity", 1.);*/
        }
	}
	
};


}
