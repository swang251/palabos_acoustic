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

/** \file
 * Definition of external fields for a lattice descriptor -- header file
 */
#ifndef EXTERNAL_FIELDS_H
#define EXTERNAL_FIELDS_H

#include "core/globalDefs.h"

namespace plb {

namespace descriptors {

struct NoExternalField {
    static const int numScalars = 0;
    static const int numSpecies = 0;
    static const int forceBeginsAt = 0;
    static const int sizeOfForce   = 0;
};

struct NoExternalFieldBase {
    typedef NoExternalField ExternalField;
};

struct Force2dDescriptor {
    static const int numScalars = 2;
    static const int numSpecies = 1;
    static const int forceBeginsAt = 0;
    static const int sizeOfForce   = 2;
};

struct Force2dDescriptorBase {
    typedef Force2dDescriptor ExternalField;
};

struct Velocity2dDescriptor {
    static const int numScalars = 2;
    static const int numSpecies = 1;
    static const int velocityBeginsAt = 0;
    static const int sizeOfVelocity   = 2;
    static const int sizeOfForce    = 0;
};

struct Velocity2dDescriptorBase {
    typedef Velocity2dDescriptor ExternalField;
};



struct VelocityAndScalar2dDescriptor {
    static const int numScalars = 3;
    static const int numSpecies = 2;
    static const int velocityBeginsAt = 0;
    static const int sizeOfVelocity   = 2;
    static const int scalarBeginsAt   = 2;
    static const int sizeOfScalar     = 1;
    static const int sizeOfForce      = 0;
};

struct VelocityAndScalar2dBase {
    typedef VelocityAndScalar2dDescriptor ExternalField;
};



struct RhoBarJdescriptor2D {
    static const int numScalars     = 3;
    static const int numSpecies     = 2;
    static const int rhoBarBeginsAt = 0;
    static const int sizeOfRhoBar   = 1;
    static const int jBeginsAt      = 1;
    static const int sizeOfJ        = 2;
    static const int sizeOfForce    = 0;
};

struct RhoBarJdescriptorBase2D {
    typedef RhoBarJdescriptor2D ExternalField;
};

struct RhoBarJPiNeqOmegaDescriptor2D {
    static const int numScalars       = 7;
    static const int numSpecies       = 4;
    static const int rhoBarBeginsAt   = 0;
    static const int sizeOfRhoBar     = 1;
    static const int velocityBeginsAt = 1; // j is stored here
    static const int sizeOfVelocity   = 2;
    static const int piNeqBeginsAt    = 3;
    static const int sizeOfPiNeq      = 3;
    static const int omegaBeginsAt    = 6;
    static const int sizeOfOmega      = 1;
    static const int sizeOfForce      = 0;
};

struct RhoBarJPiNeqOmegaDescriptorBase2D {
    typedef RhoBarJPiNeqOmegaDescriptor2D ExternalField;
};


struct ForcedRhoBarJdescriptor2D {
    static const int numScalars     = 5;
    static const int numSpecies     = 3;
    static const int rhoBarBeginsAt = 0;
    static const int sizeOfRhoBar   = 1;
    static const int jBeginsAt      = 1;
    static const int sizeOfJ        = 2;
    static const int forceBeginsAt  = 3;
    static const int sizeOfForce    = 2;
};

struct ForcedRhoBarJdescriptorBase2D {
    typedef ForcedRhoBarJdescriptor2D ExternalField;
};

struct AbsorbingWaveDescriptor2D {
    static const int numScalars             = 5;
    static const int numSpecies             = 4;
    static const int sigmaBeginsAt          = 0;
    static const int sizeOfSigma            = 1;
    static const int invDenominatorBeginsAt = 1;
    static const int sizeOfInvDenominator   = 1;
    static const int rhoBarBeginsAt         = 2;
    static const int sizeOfRhoBar           = 1;
    static const int uBeginsAt              = 3; 
    static const int sizeOfU                = 2;
    static const int sizeOfForce            = 0;
};

struct AbsorbingWaveExternalField2dBase {
    typedef AbsorbingWaveDescriptor2D ExternalField;
};

struct Force3dDescriptor {
    static const int numScalars = 3;
    static const int numSpecies = 1;
    static const int forceBeginsAt = 0;
    static const int sizeOfForce   = 3;
};

struct Force3dDescriptorBase {
    typedef Force3dDescriptor ExternalField;
};

struct RhoBarJdescriptor3D {
    static const int numScalars     = 4;
    static const int numSpecies     = 2;
    static const int rhoBarBeginsAt = 0;
    static const int sizeOfRhoBar   = 1;
    static const int jBeginsAt      = 1;
    static const int sizeOfJ        = 3;
    static const int sizeOfForce    = 0;
};

struct RhoBarJdescriptorBase3D {
    typedef RhoBarJdescriptor3D ExternalField;
};


struct ForcedRhoBarJdescriptor3D {
    static const int numScalars     = 7;
    static const int numSpecies     = 3;
    static const int rhoBarBeginsAt = 0;
    static const int sizeOfRhoBar   = 1;
    static const int jBeginsAt      = 1;
    static const int sizeOfJ        = 3;
    static const int forceBeginsAt  = 4;
    static const int sizeOfForce    = 3;
};

struct ForcedRhoBarJdescriptorBase3D {
    typedef ForcedRhoBarJdescriptor3D ExternalField;
};


struct Velocity3dDescriptor {
    static const int numScalars = 3;
    static const int numSpecies = 1;
    static const int velocityBeginsAt = 0;
    static const int sizeOfVelocity   = 3;
    static const int sizeOfForce    = 0;
};

struct Velocity3dBase {
    typedef Velocity3dDescriptor ExternalField;
};

struct VelocityAndScalar3dDescriptor {
    static const int numScalars = 4;
    static const int numSpecies = 2;
    static const int velocityBeginsAt = 0;
    static const int sizeOfVelocity   = 3;
    static const int scalarBeginsAt   = 3;
    static const int sizeOfScalar     = 1;
    static const int sizeOfForce      = 0;
};

struct SymTensor3dDescriptor {
    static const int numScalars = 6;
    static const int numSpecies = 1;
    static const int tensorBeginsAt = 0;
    static const int sizeOfTensor   = 6;
    static const int sizeOfForce    = 0;
};

struct SymTensor3dBase {
    typedef SymTensor3dDescriptor ExternalField;
};

struct VelocityAndScalar3dBase {
    typedef VelocityAndScalar3dDescriptor ExternalField;
};

struct AbsorbingWaveDescriptor3D {
    static const int numScalars             = 6;
    static const int numSpecies             = 4;
    static const int sigmaBeginsAt          = 0;
    static const int sizeOfSigma            = 1;
    static const int invDenominatorBeginsAt = 1;
    static const int sizeOfInvDenominator   = 1;
    static const int rhoBarBeginsAt         = 2;
    static const int sizeOfRhoBar           = 1;
    static const int uBeginsAt              = 3; 
    static const int sizeOfU                = 3;
    static const int sizeOfForce            = 0;
};

struct AbsorbingWaveExternalField3dBase {
    typedef AbsorbingWaveDescriptor3D ExternalField;
};

}  // namespace descriptors

}  // namespace plb

#endif
