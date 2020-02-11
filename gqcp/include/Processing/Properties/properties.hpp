// This file is part of GQCG-gqcp.
// 
// Copyright (C) 2017-2019  the GQCG developers
// 
// GQCG-gqcp is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// GQCG-gqcp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with GQCG-gqcp.  If not, see <http://www.gnu.org/licenses/>.
// 
#pragma once


#include "QCModel/CI/LinearExpansion.hpp"
#include "Operator/SecondQuantized/SQOneElectronOperator.hpp"
#include "Processing/RDM/OneRDM.hpp"


namespace GQCP {


/**
 *  Calculate the electric polarizability from the linear wave function response
 * 
 *  @param F_p          the electric response force (d^2E/dFdp)
 *  @param response     the linear wave function response
 * 
 *  @return the components of the electric polarizability
 */
Matrix<double, 3, 3> calculateElectricPolarizability(const Matrix<double, Dynamic, 3>& F_p, const Matrix<double, Dynamic, 3>& response);

/**
 *  Calculate the Dyson 'amplitudes' (the coefficients of a Dyson orbital) between two wave function expressed in the same spinor basis 
 * 
 *  @param linear_expansion1        a wave function in a spin-unresolved ONV basis  
 *  @param linear_expansion2        a wave function in a spin-unresolved ONV basis containing one fewer electron and the same amount of orbitals that is expressed in the same basis
 *
 *  @return a vector with the Dyson orbital amplitudes  
 */
VectorX<double> calculateDysonOrbitalCoefficients(const LinearExpansion& linear_expansion1, const LinearExpansion& linear_expansion2);


}  // namespace GQCP