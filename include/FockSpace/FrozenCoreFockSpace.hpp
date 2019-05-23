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
#ifndef GQCP_FROZENCOREFOCKSPACE_HPP
#define GQCP_FROZENCOREFOCKSPACE_HPP


#include "FockSpace/BaseFockSpace.hpp"


namespace GQCP {


struct FrozenOperators {
    OneElectronOperator<double> one_op;
    TwoElectronOperator<double> two_op;
};


/**
 *  A class that represents a frozen Fock space: this is a subspace of the N-electron Fock space in which the first X orbitals are always occupied
 */
class FrozenCoreFockSpace: public BaseFockSpace {
protected:

    size_t X;  // number of frozen orbitals/electrons
    std::shared_ptr<BaseFockSpace> active_fock_space;  // active (non-frozen) Fock space containing only the active electrons (N-X) and orbitals (K-X)

public:
    // CONSTRUCTORS
    /**
     *  @param fock_space                    shared pointer to active (non-frozen core) Fock space
     *  @param X                             the number of frozen orbitals
     */
    FrozenCoreFockSpace(std::shared_ptr<BaseFockSpace> fock_space, size_t X);

    /**
     *  Evaluate the operator in a dense matrix
     *
     *  @param one_op               the one-electron operator to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the operator's evaluation in a dense matrix with the dimensions of the Fock space
     */
    SquareMatrix<double> evaluateOperatorDense(const OneElectronOperator<double>& one_op, bool diagonal_values) const override;

    /**
     *  Evaluate the operator in a sparse matrix
     *
     *  @param one_op               the one-electron operator to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the operator's evaluation in a sparse matrix with the dimensions of the Fock space
     */
    Eigen::SparseMatrix<double> evaluateOperatorSparse(const OneElectronOperator<double>& one_op,
                                                       bool diagonal_values) const override;

    /**
     *  Evaluate the operator in a dense matrix
     *
     *  @param two_op               the two-electron operator to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the operator's evaluation in a dense matrix with the dimensions of the Fock space
     */
    SquareMatrix<double> evaluateOperatorDense(const TwoElectronOperator<double>& two_op, bool diagonal_values) const override;

    /**
     *  Evaluate the operator in a sparse matrix
     *
     *  @param two_op               the two-electron operator to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the operator's evaluation in a sparse matrix with the dimensions of the Fock space
     */
    Eigen::SparseMatrix<double> evaluateOperatorSparse(const TwoElectronOperator<double>& two_op,
                                                       bool diagonal_values) const override;

    /**
     *  Evaluate the Hamiltonian in a dense matrix
     *
     *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the Hamiltonian's evaluation in a dense matrix with the dimensions of the Fock space
     */
    SquareMatrix<double> evaluateOperatorDense(const HamiltonianParameters<double>& ham_par,
                                               bool diagonal_values) const override;

    /**
     *  Evaluate the Hamiltonian in a sparse matrix
     *
     *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
     *  @param diagonal_values      bool to indicate if diagonal values will be calculated
     *
     *  @return the Hamiltonian's evaluation in a sparse matrix with the dimensions of the Fock space
     */
    Eigen::SparseMatrix<double> evaluateOperatorSparse(const HamiltonianParameters<double>& ham_par,
                                                       bool diagonal_values) const override;

    /**
     *  Evaluate the diagonal of the operator
     *
     *  @param one_op               the one-electron operator to be evaluated in the Fock space
     *
     *  @return the operator's diagonal evaluation in a vector with the dimension of the Fock space
     */
    VectorX<double> evaluateOperatorDiagonal(const OneElectronOperator<double>& one_op) const override;

    /**
     *  Evaluate the diagonal of the operator
     *
     *  @param two_op               the two-electron operator to be evaluated in the Fock space
     *
     *  @return the operator's diagonal evaluation in a vector with the dimension of the Fock space
     */
    VectorX<double> evaluateOperatorDiagonal(const TwoElectronOperator<double>& two_op) const override;

    /**
     *  Evaluate the diagonal of the Hamiltonian
     *
     *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
     *
     *  @return the Hamiltonian's diagonal evaluation in a vector with the dimension of the Fock space
     */
    VectorX<double> evaluateOperatorDiagonal(const HamiltonianParameters<double>& ham_par) const override;

    /**
     *  @param one_op       the one-electron operator in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return 'frozen' one-electron operator which cover evaluations from the active and inactive orbitals
     */
    OneElectronOperator<double> freezeOperator(const OneElectronOperator<double>& one_op, size_t X) const;

    /**
     *  @param two_op       the two-electron operator in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return 'frozen' two-electron operator which cover evaluations from the active and inactive orbitals
     */
    FrozenOperators freezeOperator(const TwoElectronOperator<double>& two_op, size_t X) const;

    // PUBLIC METHODS
    /**
     *  @param ham_par      the Hamiltonian parameters in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return a set of 'frozen' Hamiltonian parameters which cover two-electron integral evaluations from the active and inactive orbitals
     *  (see https://drive.google.com/file/d/1Fnhv2XyNO9Xw9YDoJOXU21_6_x2llntI/view?usp=sharing)
     */
    HamiltonianParameters<double> freezeOperator(const HamiltonianParameters<double>& ham_par, size_t X) const;

    /**
     *  @param one_op       the one-electron operator in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return the operator diagonal from strictly evaluating the frozen orbitals in the Fock space
     */
    VectorX<double> frozenCoreDiagonal(const OneElectronOperator<double>& one_op, size_t X) const;

    /**
     *  @param two_op       the two-electron operator in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return the operator diagonal from strictly evaluating the frozen orbitals in the Fock space
     */
    VectorX<double> frozenCoreDiagonal(const TwoElectronOperator<double>& two_op, size_t X) const;

    /**
     *  @param ham_par      the Hamiltonian parameters in an orthonormal orbital basis
     *  @param X            the number of frozen orbitals
     *
     *  @return the Hamiltonian diagonal from strictly evaluating the frozen orbitals in the Fock space
     */
    VectorX<double> frozenCoreDiagonal(const HamiltonianParameters<double>& ham_par, size_t X) const;
};


}  // namespace GQCP


#endif  // GQCP_FROZENCOREFOCKSPACE_HPP
