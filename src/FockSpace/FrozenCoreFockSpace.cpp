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
#include "FockSpace/FrozenCoreFockSpace.hpp"


namespace GQCP {



FrozenCoreFockSpace::FrozenCoreFockSpace(std::shared_ptr<GQCP::BaseFockSpace> fock_space, size_t X) :
    BaseFockSpace(fock_space->get_K(), fock_space->get_dimension()),
    active_fock_space (std::move(fock_space)),
    X (X)
{}

/**
 *  Evaluate the operator in a dense matrix
 *
 *  @param one_op               the one-electron operator to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the operator's evaluation in a dense matrix with the dimensions of the Fock space
 */
SquareMatrix<double> FrozenCoreFockSpace::evaluateOperatorDense(const OneElectronOperator<double>& one_op, bool diagonal_values) const {

    // Freeze Hamiltonian parameters
    OneElectronOperator<double> frozen_one_op = this->freezeOperator(one_op, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorDense(frozen_one_op, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_one_op, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();

    }

    return evaluation;
}


/**
 *  Evaluate the operator in a sparse matrix
 *
 *  @param one_op               the one-electron operator to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the operator's evaluation in a sparse matrix with the dimensions of the Fock space
 */
Eigen::SparseMatrix<double> FrozenCoreFockSpace::evaluateOperatorSparse(const OneElectronOperator<double>& one_op,
                                                   bool diagonal_values) const {

    // Freeze the operator
    OneElectronOperator<double> frozen_one_op = this->freezeOperator(one_op, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorSparse(frozen_one_op, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_one_op, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();

    }

    return evaluation;
}


/**
 *  Evaluate the operator in a dense matrix
 *
 *  @param two_op               the two-electron operator to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the operator's evaluation in a dense matrix with the dimensions of the Fock space
 */
SquareMatrix<double> FrozenCoreFockSpace::evaluateOperatorDense(const TwoElectronOperator<double>& two_op, bool diagonal_values) const {

    // Freeze the operators
    auto frozen_ops = this->freezeOperator(two_op, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorDense(frozen_ops.one_op, diagonal_values);
    evaluation += this->active_fock_space->evaluateOperatorDense(frozen_ops.two_op, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ops.one_op, this->X);
        frozen_core_diagonal += this->frozenCoreDiagonal(frozen_ops.two_op, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();
    }

    return evaluation;

}

/**
 *  Evaluate the operator in a sparse matrix
 *
 *  @param two_op               the two-electron operator to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the operator's evaluation in a sparse matrix with the dimensions of the Fock space
 */
Eigen::SparseMatrix<double> FrozenCoreFockSpace::evaluateOperatorSparse(const TwoElectronOperator<double>& two_op,
                                                   bool diagonal_values) const {

    // Freeze the operators
    auto frozen_ops = this->freezeOperator(two_op, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorSparse(frozen_ops.one_op, diagonal_values);
    evaluation += this->active_fock_space->evaluateOperatorSparse(frozen_ops.two_op, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ops.one_op, this->X);
        frozen_core_diagonal += this->frozenCoreDiagonal(frozen_ops.two_op, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();
    }

    return evaluation;

}

/**
 *  Evaluate the Hamiltonian in a dense matrix
 *
 *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the Hamiltonian's evaluation in a dense matrix with the dimensions of the Fock space
 */
SquareMatrix<double> FrozenCoreFockSpace::evaluateOperatorDense(const HamiltonianParameters<double>& ham_par,
                                           bool diagonal_values) const  {
    // Freeze the operators
    auto frozen_ham_par = this->freezeOperator(ham_par, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorDense(frozen_ham_par, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ham_par, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();

    }

    return evaluation;
}

/**
 *  Evaluate the Hamiltonian in a sparse matrix
 *
 *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
 *  @param diagonal_values      bool to indicate if diagonal values will be calculated
 *
 *  @return the Hamiltonian's evaluation in a sparse matrix with the dimensions of the Fock space
 */
Eigen::SparseMatrix<double> FrozenCoreFockSpace::evaluateOperatorSparse(const HamiltonianParameters<double>& ham_par,
                                                   bool diagonal_values) const  {
    // Freeze the operators
    auto frozen_ham_par = this->freezeOperator(ham_par, this->X);

    // calculate Hamiltonian matrix through conventional CI
    auto evaluation = this->active_fock_space->evaluateOperatorSparse(frozen_ham_par, diagonal_values);

    if (diagonal_values) {
        // diagonal correction
        VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
        auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ham_par, this->X);
        evaluation += frozen_core_diagonal.asDiagonal();

    }

    return evaluation;
}

/**
 *  Evaluate the diagonal of the operator
 *
 *  @param one_op               the one-electron operator to be evaluated in the Fock space
 *
 *  @return the operator's diagonal evaluation in a vector with the dimension of the Fock space
 */
VectorX<double> FrozenCoreFockSpace::evaluateOperatorDiagonal(const OneElectronOperator<double>& one_op) const {

    auto frozen_op = this->freezeOperator(one_op, this->X);

    // calculate diagonal in the active space with the "frozen" Hamiltonian parameters
    auto diagonal = this->active_fock_space->evaluateOperatorDiagonal(frozen_op);

    // calculate diagonal for the frozen orbitals
    auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_op, this->X);

    return diagonal + frozen_core_diagonal;
};

/**
 *  Evaluate the diagonal of the operator
 *
 *  @param two_op               the two-electron operator to be evaluated in the Fock space
 *
 *  @return the operator's diagonal evaluation in a vector with the dimension of the Fock space
 */
VectorX<double> FrozenCoreFockSpace::evaluateOperatorDiagonal(const TwoElectronOperator<double>& two_op) const {
    auto frozen_ops = this->freezeOperator(two_op, this->X);

    // calculate diagonal in the active space with the "frozen" Hamiltonian parameters
    auto diagonal = this->active_fock_space->evaluateOperatorDiagonal(frozen_ops.two_op);
    diagonal += this->active_fock_space->evaluateOperatorDiagonal(frozen_ops.one_op);

    // calculate diagonal for the frozen orbitals
    auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ops.two_op, this->X);
    frozen_core_diagonal += this->frozenCoreDiagonal(frozen_ops.one_op, this->X);

    return diagonal + frozen_core_diagonal;
}

/**
 *  Evaluate the diagonal of the Hamiltonian
 *
 *  @param ham_par              HamiltonianParameters to be evaluated in the Fock space
 *
 *  @return the Hamiltonian's diagonal evaluation in a vector with the dimension of the Fock space
 */
VectorX<double> FrozenCoreFockSpace::evaluateOperatorDiagonal(const HamiltonianParameters<double>& ham_par) const {
    auto frozen_ham_par = this->freezeOperator(ham_par, this->X);

    // calculate diagonal in the active space with the "frozen" Hamiltonian parameters
    auto diagonal = this->active_fock_space->evaluateOperatorDiagonal(frozen_ham_par);

    // calculate diagonal for the frozen orbitals
    auto frozen_core_diagonal = this->frozenCoreDiagonal(frozen_ham_par, this->X);

    return diagonal + frozen_core_diagonal;
}

/**
 *  @param one_op       the one-electron operator in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return 'frozen' one-electron operator which cover evaluations from the active and inactive orbitals
 */
OneElectronOperator<double> FrozenCoreFockSpace::freezeOperator(const OneElectronOperator<double>& one_op, size_t X) const {

    size_t K_active = this->K - X;
    return OneElectronOperator<double>(one_op.block(X, X, K_active, K_active));
}

/**
 *  @param two_op       the two-electron operator in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return 'frozen' two-electron operator which cover evaluations from the active and inactive orbitals
 */
FrozenOperators FrozenCoreFockSpace::freezeOperator(const TwoElectronOperator<double>& two_op, size_t X) const {

    size_t K_active = this->K - X;
    OneElectronOperator<double> frozen_one_op = OneElectronOperator<double>::Zero(K_active, K_active);
    auto frozen_two_op = TwoElectronOperator<double>::FromBlock(two_op, X, X, X, X);

    // Frozen two-electron integrals can be rewritten partially as one electron integrals.
    for (size_t i = 0; i < K_active; i++) {  // iterate over the active orbitals

        size_t q = i + X;  // map active orbitals indexes to total orbital indexes (those including the frozen orbitals)

        for (size_t l = 0; l < X; l++) {  // iterate over the frozen orbitals
            frozen_one_op(i,i) += two_op(q,q,l,l) + two_op(l,l,q,q) - two_op(q,l,l,q)/2 - two_op(l,q,q,l)/2;
        }

        for (size_t j = i+1; j < K_active; j++) {  // iterate over the active orbitals

            size_t p = j + X;  // map active orbitals indexes to total orbital indexes (those including the frozen orbitals)

            for (size_t l = 0; l < X; l++) {  // iterate over the frozen orbitals

                frozen_one_op(i,j) += two_op(q,p,l,l) + two_op(l,l,q,p) - two_op(q,l,l,p)/2 - two_op(l,p,q,l)/2;
                frozen_one_op(j,i) += two_op(p,q,l,l) + two_op(l,l,p,q) - two_op(p,l,l,q)/2 - two_op(l,q,p,l)/2;
            }
        }
    }

    return {frozen_one_op, frozen_two_op};
}

// PUBLIC METHODS
/**
 *  @param ham_par      the Hamiltonian parameters in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return a set of 'frozen' Hamiltonian parameters which cover two-electron integral evaluations from the active and inactive orbitals
 *  (see https://drive.google.com/file/d/1Fnhv2XyNO9Xw9YDoJOXU21_6_x2llntI/view?usp=sharing)
 */
HamiltonianParameters<double> FrozenCoreFockSpace::freezeOperator(const HamiltonianParameters<double>& ham_par, size_t X) const {

    size_t K_active = this->K - X;  // number of non-frozen orbitals


    // Copy one-electron integrals from the non-frozen orbitals
    OneElectronOperator<double> S (ham_par.get_S().block(X, X, K_active, K_active));  // active
    OneElectronOperator<double> h (ham_par.get_h().block(X, X, K_active, K_active));  // active

    // Copy two-electron integrals from the non-frozen orbitals
    const auto& g = ham_par.get_g();  // total

    // 'Freeze' the Hamiltonian parameters
    // This amounts to modifying the (active) one-electron integrals using derived formulas
    for (size_t i = 0; i < K_active; i++) {  // iterate over the active orbitals

        size_t q = i + X;  // map active orbitals indexes to total orbital indexes (those including the frozen orbitals)

        for (size_t l = 0; l < X; l++) {  // iterate over the frozen orbitals
            h(i,i) += g(q,q,l,l) + g(l,l,q,q) - g(q,l,l,q)/2 - g(l,q,q,l)/2;
        }

        for (size_t j = i+1; j < K_active; j++) {  // iterate over the active orbitals

            size_t p = j + X;  // map active orbitals indexes to total orbital indexes (those including the frozen orbitals)

            for (size_t l = 0; l < X; l++) {  // iterate over the frozen orbitals

                h(i,j) += g(q,p,l,l) + g(l,l,q,p) - g(q,l,l,p)/2 - g(l,p,q,l)/2;
                h(j,i) += g(p,q,l,l) + g(l,l,p,q) - g(p,l,l,q)/2 - g(l,q,p,l)/2;
            }
        }
    }

    std::shared_ptr<AOBasis> ao_basis;  // nullptr
    auto g_new = TwoElectronOperator<double>::FromBlock(ham_par.get_g(), X, X, X, X);
    SquareMatrix<double> T = ham_par.get_T_total().block(X, X, K_active, K_active);

    return HamiltonianParameters<double>(ao_basis, S, h, g_new, T);
}

/**
 *  @param one_op       the one-electron operator in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return the operator diagonal from strictly evaluating the frozen orbitals in the Fock space
 */
VectorX<double> FrozenCoreFockSpace::frozenCoreDiagonal(const OneElectronOperator<double>& one_op, size_t X) const {

    // The diagonal value for the frozen orbitals is the same for each ONV
    double value = 0;
    for (size_t i = 0; i < X; i++) {

        value += 2*one_op(i,i);

    }

    VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
    return value * diagonal;

}

/**
 *  @param two_op       the two-electron operator in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return the operator diagonal from strictly evaluating the frozen orbitals in the Fock space
 */
VectorX<double> FrozenCoreFockSpace::frozenCoreDiagonal(const TwoElectronOperator<double>& two_op, size_t X) const {

    // The diagonal value for the frozen orbitals is the same for each ONV
    double value = 0;
    for (size_t i = 0; i < X; i++) {

        value += two_op(i,i,i,i);

        for (size_t j = i+1; j < X; j++) {

            value += 2*two_op(i,i,j,j) + 2*two_op(j,j,i,i) - two_op(j,i,i,j) - two_op(i,j,j,i);
        }
    }

    VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
    return value * diagonal;
}

/**
 *  @param ham_par      the Hamiltonian parameters in an orthonormal orbital basis
 *  @param X            the number of frozen orbitals
 *
 *  @return the Hamiltonian diagonal from strictly evaluating the frozen orbitals in the Fock space
 */
VectorX<double> FrozenCoreFockSpace::frozenCoreDiagonal(const HamiltonianParameters<double>& ham_par, size_t X) const {

    const auto& g = ham_par.get_g();
    const auto& h = ham_par.get_h();

    // The diagonal value for the frozen orbitals is the same for each ONV
    double value = 0;
    for (size_t i = 0; i < X; i++) {

        value += 2*h(i,i) + g(i,i,i,i);

        for (size_t j = i+1; j < X; j++) {

            value += 2*g(i,i,j,j) + 2*g(j,j,i,i) - g(j,i,i,j) - g(i,j,j,i);
        }
    }

    VectorX<double> diagonal = VectorX<double>::Ones(this->dim);
    return value * diagonal;
}


}  // namespace GQCP
