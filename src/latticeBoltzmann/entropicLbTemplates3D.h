/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2015 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Orestis Malaspinas contributed this code.
 */

/** \file
 * A collection of dynamics classes (e.g. BGK) with which a Cell object
 * can be instantiated -- header file.
 */
#ifndef ENTROPIC_LB_HELPERS_3D_H
#define ENTROPIC_LB_HELPERS_3D_H

#include "core/globalDefs.h"
#include "latticeBoltzmann/nearestNeighborLattices3D.h"
#include <cmath>

namespace plb {

template<typename T>
struct entropicLbTemplates<T, descriptors::D3Q19Descriptor>
{
    /// Computation of equilibrium distribution with an expansion
    /// with respect to a small velocity u
    static T equilibrium( plint iPop, T rho, Array<T,3> const& u)
    {
        typedef descriptors::D3Q19Descriptor<T> L;

        T c_u = L::c[iPop][0]*u[0] + L::c[iPop][1]*u[1] + L::c[iPop][2]*u[2];;
        T c_u2 = c_u*c_u;
        T c_u3 = c_u2*c_u;
        T c_u4 = c_u3*c_u;
        T c_u5 = c_u4*c_u;
        T c_u6 = c_u5*c_u;
        T c_u7 = c_u6*c_u;
        
        T uSqr = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];
        T uSqr2 = uSqr*uSqr;
        T uSqr3 = uSqr2*uSqr;
        
        T powUx = u[0]*u[0]*u[0]*u[0]*u[0]; // u_x^5
        T powUy = u[1]*u[1]*u[1]*u[1]*u[1]; // u_y^5
        T powUz = u[2]*u[2]*u[2]*u[2]*u[2]; // u_z^5
        
        T C = L::c[iPop][0] * powUx + L::c[iPop][1] * powUy
                + L::c[iPop][2] * powUz;
        
        powUx *= u[0]; // u_x^6
        powUy *= u[1]; // u_y^6
        powUz *= u[2]; // u_z^6
        
        T E = powUx + powUy + powUz;
        
        powUx *= u[0]; // u_x^7
        powUy *= u[1]; // u_y^7
        powUz *= u[2]; // u_z^7
        
        T F = L::c[iPop][0] * powUx + L::c[iPop][1] * powUy + L::c[iPop][2] * powUz;
        
        return L::t[iPop] * rho * 
              ((T)1
                + c_u*(C*(T)81/(T)20 + uSqr2*(T)27/(T)8 - uSqr*(T)9/(T)2
                - E*(T)81/(T)24 - uSqr3*(T)81/(T)48 + (T)3)
                
                + c_u2*(uSqr2*(T)81/(T)16 - uSqr*(T)27/(T)4 
                + C*(T)243/(T)40 + (T)9/(T)2)
                
                + c_u3*(uSqr2*(T)243/(T)48 - uSqr*(T)81/(T)12 + (T)27/(T)6)
                
                - c_u4*uSqr*(T)243/(T)48
                + c_u4*(T)81/(T)24
                
                - c_u5*uSqr*(T)729/(T)240
                + c_u5*(T)243/(T)120
                
                + c_u6*(T)729/(T)720
                
                + c_u7*(T)2187/(T)5040
                
                - C*uSqr*(T)81/(T)40
                
                + C*(T)27/(T)20 - uSqr3*(T)27/(T)48 - E*(T)27/(T)24
                - F*(T)81/(T)56 - uSqr*(T)3/(T)2 + uSqr2*(T)9/(T)8
                )
                - L::SkordosFactor()*L::t[iPop];
    }
    
};

}

#endif
