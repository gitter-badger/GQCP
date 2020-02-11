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


#include "ONVBasis/SpinResolvedONVBasis.hpp"
#include "QCMethod/CI/HamiltonianBuilder/HamiltonianBuilder.hpp"


namespace GQCP {


/**
 *  Hubbard builds a Hubbard Hamiltonian matrix in the full spin-resolved ONV basis
 *
 *  Hubbard distinguishes itself from FCI by explicitly implementing simplified a Hamiltonian:
 *      - for the one electron operators only inter-site interactions are considered
 *      - for the two electron operators only on-site (doubly occupied in-place) interactions are considered
 */
class Hubbard : public HamiltonianBuilder {
private:
    SpinResolvedONVBasis onv_basis;  // the full spin-resolved ONV basis


public:

    // CONSTRUCTORS
    /**
     *  @param onv_basis       the full spin-resolved ONV basis
     */
    explicit Hubbard(const SpinResolvedONVBasis& onv_basis);


    // DESTRUCTOR
    ~Hubbard() = default;


    // OVERRIDDEN GETTERS
    const BaseONVBasis* get_fock_space() const override { return &onv_basis; }


    // OVERRIDDEN PUBLIC METHODS
    /**
     *  @param sq_hamiltonian               the Hubbard Hamiltonian expressed in an orthonormal basis
     *
     *  @return the Hubbard Hamiltonian matrix
     */
    SquareMatrix<double> constructHamiltonian(const SQHamiltonian<double>& sq_hamiltonian) const override;

    /**
     *  @param sq_hamiltonian               the Hubbard Hamiltonian expressed in an orthonormal basis
     *  @param x                            the vector upon which the Hubbard Hamiltonian acts
     *  @param diagonal                     the diagonal of the Hubbard Hamiltonian matrix
     *
     *  @return the action of the Hubbard Hamiltonian on the coefficient vector
     */
    VectorX<double> matrixVectorProduct(const SQHamiltonian<double>& sq_hamiltonian, const VectorX<double>& x, const VectorX<double>& diagonal) const override;

    /**
     *  @param sq_hamiltonian               the Hubbard Hamiltonian expressed in an orthonormal basis
     *
     *  @return the diagonal of the matrix representation of the Hubbard Hamiltonian
     */
    VectorX<double> calculateDiagonal(const SQHamiltonian<double>& sq_hamiltonian) const override;
};



}  // namespace GQCP