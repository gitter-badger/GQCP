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


#include "Mathematical/Optimization/Eigenproblem/EigenproblemEnvironment.hpp"
#include "ONVBasis/FrozenONVBasis.hpp"
#include "ONVBasis/FrozenProductONVBasis.hpp"
#include "ONVBasis/ONVBasis.hpp"
#include "ONVBasis/ProductONVBasis.hpp"
#include "ONVBasis/SelectedONVBasis.hpp"
#include "Operator/SecondQuantized/SQHamiltonian.hpp"


namespace GQCP {
namespace CIEnvironment {


/**
 *  Create an iterative eigenvalue problem environment for spin-resolved FCI calculations.
 */
EigenproblemEnvironment Dense(const SQHamiltonian<double>& sq_hamiltonian, const ProductONVBasis& onv_basis) {

    const auto H = onv_basis.evaluateOperatorDense(sq_hamiltonian, true);  // 'true': with calculation of the diagonal
    return EigenproblemEnvironment::Dense(H);
}


/**
 *  Create an iterative eigenvalue problem environment for spin-resolved FCI calculations.
 */
// EigenproblemEnvironment Iterative(const SQHamiltonian<double>& sq_hamiltonian, const ProductONVBasis& onv_basis) {

//     const auto diagonal = onv_basis.evaluateOperatorDiagonal(sq_hamiltonian);


//     // 

//     return EigenproblemEnvironment::Iterative()
// }


}  // namespace CIEnvironment
}  // namespace GQCP
