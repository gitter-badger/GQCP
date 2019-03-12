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
#include "HamiltonianBuilder/SelectedCI.hpp"


namespace GQCP {

/*
 *  PRIVATE METHODS
 */

void SelectedCI::evaluateHamiltonianElements(const HamiltonianParameters<double>& hamiltonian_parameters, const PassToMethod& method) const {

    size_t dim = fock_space.get_dimension();
    size_t K = fock_space.get_K();

    auto h = hamiltonian_parameters.get_h();
    auto g = hamiltonian_parameters.get_g();

    for (size_t I = 0; I < dim; I++) {  // loop over all addresses (1)
        Configuration configuration_I = this->fock_space.get_configuration(I);
        ONV alpha_I = configuration_I.onv_alpha;
        ONV beta_I = configuration_I.onv_beta;

        // Calculate the off-diagonal elements, by going over all other ONVs
        for (size_t J = I+1; J < dim; J++) {

            Configuration configuration_J = this->fock_space.get_configuration(J);
            ONV alpha_J = configuration_J.onv_alpha;
            ONV beta_J = configuration_J.onv_beta;

            if ((alpha_I.countNumberOfDifferences(alpha_J) == 2) && (beta_I.countNumberOfDifferences(beta_J) == 0)) {

                // Find the orbitals that are occupied in one string, and aren't in the other
                size_t p = alpha_I.findDifferentOccupations(alpha_J)[0];  // we're sure that there is only 1 element in the std::vector<size_t>
                size_t q = alpha_J.findDifferentOccupations(alpha_I)[0];  // we're sure that there is only 1 element in the std::vector<size_t>

                // Calculate the total sign
                int sign = alpha_I.operatorPhaseFactor(p) * alpha_J.operatorPhaseFactor(q);

                double value = h(p,q);

                method(I, J, sign*value);
                method(J, I, sign*value);

                for (size_t r = 0; r < K; r++) {  // r loops over spatial orbitals

                    if (alpha_I.isOccupied(r) && alpha_J.isOccupied(r)) {  // r must be occupied on the left and on the right
                        if ((p != r) && (q != r)) {  // can't create or annihilate the same orbital

                            double value = 0.5 * (g(p,q,r,r)
                                               - g(r,q,p,r)
                                               - g(p,r,r,q)
                                               + g(r,r,p,q));

                            method(I, J, sign*value);
                            method(J, I, sign*value);
                        }
                    }

                    if (beta_I.isOccupied(r)) {  // beta_I == beta_J from the previous if-branch

                        double value = 0.5 * (g(p,q,r,r)
                                           +  g(r,r,p,q));

                        method(I, J, sign*value);
                        method(J, I, sign*value);
                    }
                }
            }

            // 0 electron excitations in alpha, 1 in beta
            if ((alpha_I.countNumberOfDifferences(alpha_J) == 0) && (beta_I.countNumberOfDifferences(beta_J) == 2)) {


                // Find the orbitals that are occupied in one string, and aren't in the other
                size_t p = beta_I.findDifferentOccupations(beta_J)[0];  // we're sure that there is only 1 element in the std::vector<size_t>
                size_t q = beta_J.findDifferentOccupations(beta_I)[0];  // we're sure that there is only 1 element in the std::vector<size_t>

                // Calculate the total sign
                int sign = beta_I.operatorPhaseFactor(p) * beta_J.operatorPhaseFactor(q);

                double value = h(p,q);

                method(I, J, sign*value);
                method(J, I, sign*value);

                for (size_t r = 0; r < K; r++) {  // r loops over spatial orbitals

                    if (beta_I.isOccupied(r) && beta_J.isOccupied(r)) {  // r must be occupied on the left and on the right
                        if ((p != r) && (q != r)) {  // can't create or annihilate the same orbital
                            double value = 0.5 * (g(p,q,r,r)
                                               -  g(r,q,p,r)
                                               -  g(p,r,r,q)
                                               +  g(r,r,p,q));

                            method(I, J, sign*value);
                            method(J, I, sign*value);
                        }
                    }

                    if (alpha_I.isOccupied(r)) {  // alpha_I == alpha_J from the previous if-branch

                        double value =  0.5 * (g(p,q,r,r)
                                            +  g(r,r,p,q));

                        method(I, J, sign*value);
                        method(J, I, sign*value);
                    }
                }
            }

            // 1 electron excitation in alpha, 1 in beta
            if ((alpha_I.countNumberOfDifferences(alpha_J) == 2) && (beta_I.countNumberOfDifferences(beta_J) == 2)) {

                // Find the orbitals that are occupied in one string, and aren't in the other
                size_t p = alpha_I.findDifferentOccupations(alpha_J)[0];  // we're sure that there is only 1 element in the std::vector<size_t>
                size_t q = alpha_J.findDifferentOccupations(alpha_I)[0];  // we're sure that there is only 1 element in the std::vector<size_t>

                size_t r = beta_I.findDifferentOccupations(beta_J)[0];  // we're sure that there is only 1 element in the std::vector<size_t>
                size_t s = beta_J.findDifferentOccupations(beta_I)[0];  // we're sure that there is only 1 element in the std::vector<size_t>

                int sign = alpha_I.operatorPhaseFactor(p) * alpha_J.operatorPhaseFactor(q) * beta_I.operatorPhaseFactor(r) * beta_J.operatorPhaseFactor(s);
                double value = 0.5 * (g(p,q,r,s)
                                   +  g(r,s,p,q));

                method(I, J, sign*value);
                method(J, I, sign*value);
            }

            // 2 electron excitations in alpha, 0 in beta
            if ((alpha_I.countNumberOfDifferences(alpha_J) == 4) && (beta_I.countNumberOfDifferences(beta_J) == 0)) {

                // Find the orbitals that are occupied in one string, and aren't in the other
                std::vector<size_t> occupied_indices_I = alpha_I.findDifferentOccupations(alpha_J);  // we're sure this has two elements
                size_t p = occupied_indices_I[0];
                size_t r = occupied_indices_I[1];

                std::vector<size_t> occupied_indices_J = alpha_J.findDifferentOccupations(alpha_I);  // we're sure this has two elements
                size_t q = occupied_indices_J[0];
                size_t s = occupied_indices_J[1];

                int sign = alpha_I.operatorPhaseFactor(p) * alpha_I.operatorPhaseFactor(r) * alpha_J.operatorPhaseFactor(q) * alpha_J.operatorPhaseFactor(s);

                double value = 0.5 * (g(p,q,r,s)
                                   -  g(p,s,r,q)
                                   -  g(r,q,p,s)
                                   +  g(r,s,p,q));

                method(I, J, sign*value);
                method(J, I, sign*value);
            }

            // 0 electron excitations in alpha, 2 in beta
            if ((alpha_I.countNumberOfDifferences(alpha_J) == 0) && (beta_I.countNumberOfDifferences(beta_J) == 4)) {

                // Find the orbitals that are occupied in one string, and aren't in the other
                std::vector<size_t> occupied_indices_I = beta_I.findDifferentOccupations(beta_J);  // we're sure this has two elements
                size_t p = occupied_indices_I[0];
                size_t r = occupied_indices_I[1];

                std::vector<size_t> occupied_indices_J = beta_J.findDifferentOccupations(beta_I);  // we're sure this has two elements
                size_t q = occupied_indices_J[0];
                size_t s = occupied_indices_J[1];

                int sign = beta_I.operatorPhaseFactor(p) * beta_I.operatorPhaseFactor(r) * beta_J.operatorPhaseFactor(q) * beta_J.operatorPhaseFactor(s);

                double value = 0.5 * (g(p,q,r,s)
                                   -  g(p,s,r,q)
                                   -  g(r,q,p,s)
                                   +  g(r,s,p,q));

                method(I, J, sign*value);
                method(J, I, sign*value);
            }
        }  // loop over addresses J > I
    }  // loop over addresses I
}


/*
 *  CONSTRUCTORS
 */

/**
 *  @param fock_space       the full alpha and beta product Fock space
 */
SelectedCI::SelectedCI(const SelectedFockSpace& fock_space) :
    HamiltonianBuilder(),
    fock_space(fock_space)
{}



/*
 *  OVERRIDDEN PUBLIC METHODS
 */

/**
 *  @param hamiltonian_parameters       the SelectedCI Hamiltonian parameters in an orthonormal orbital basis
 *
 *  @return the SelectedCI Hamiltonian matrix
 */
SquareMatrix<double> SelectedCI::constructHamiltonian(const HamiltonianParameters<double>& hamiltonian_parameters) const {
    auto K = hamiltonian_parameters.get_h().get_dim();
    if (K != this->fock_space.get_K()) {
        throw std::invalid_argument("SelectedCI::constructHamiltonian(HamiltonianParameters<double>): Basis functions of the Fock space and hamiltonian_parameters are incompatible.");
    }

    auto dim = fock_space.get_dimension();

    SquareMatrix<double> result_matrix = SquareMatrix<double>::Zero(dim, dim);
    result_matrix += this->calculateDiagonal(hamiltonian_parameters).asDiagonal();

    // We should put the calculated elements inside the result matrix
    PassToMethod addToMatrix = [&result_matrix](size_t I, size_t J, double value) { result_matrix(I, J) += value; };

    this->evaluateHamiltonianElements(hamiltonian_parameters, addToMatrix);
    return result_matrix;
}


/**
 *  @param hamiltonian_parameters       the SelectedCI Hamiltonian parameters in an orthonormal orbital basis
 *  @param x                            the vector upon which the SelectedCI Hamiltonian acts
 *  @param diagonal                     the diagonal of the SelectedCI Hamiltonian matrix
 *
 *  @return the action of the SelectedCI Hamiltonian on the coefficient vector
 */
VectorX<double> SelectedCI::matrixVectorProduct(const HamiltonianParameters<double>& hamiltonian_parameters, const VectorX<double>& x, const VectorX<double>& diagonal) const {

    auto K = hamiltonian_parameters.get_h().get_dim();
    if (K != this->fock_space.get_K()) {
        throw std::invalid_argument("SelectedCI::matrixVectorProduct(HamiltonianParameters<double>, VectorX<double>, VectorX<double>): Basis functions of the Fock space and hamiltonian_parameters are incompatible.");
    }

    VectorX<double> matvec = diagonal.cwiseProduct(x);

    // We should pass the calculated elements to the resulting vector and perform the product
    PassToMethod addToMatvec = [&matvec, &x](size_t I, size_t J, double value) { matvec(I) += value * x(J); };

    this->evaluateHamiltonianElements(hamiltonian_parameters, addToMatvec);

    return matvec;
}


/**
 *  @param hamiltonian_parameters       the SelectedCI Hamiltonian parameters in an orthonormal orbital basis
 *
 *  @return the diagonal of the matrix representation of the SelectedCI Hamiltonian
 */
VectorX<double> SelectedCI::calculateDiagonal(const HamiltonianParameters<double>& hamiltonian_parameters) const {

    auto K = hamiltonian_parameters.get_h().get_dim();
    if (K != this->fock_space.get_K()) {
        throw std::invalid_argument("SelectedCI::calculateDiagonal(HamiltonianParameters<double>): Basis functions of the Fock space and hamiltonian_parameters are incompatible.");
    }

    auto dim = fock_space.get_dimension();

    auto h = hamiltonian_parameters.get_h();
    auto g = hamiltonian_parameters.get_g();

    // Diagonal contributions
    VectorX<double> diagonal = VectorX<double>::Zero(dim);

    for (size_t I = 0; I < dim; I++) {  // Ia loops over addresses of alpha onvs
        Configuration configuration_I = this->fock_space.get_configuration(I);
        ONV alpha_I = configuration_I.onv_alpha;
        ONV beta_I = configuration_I.onv_beta;

        for (size_t p = 0; p < K; p++) {
            if (alpha_I.isOccupied(p)) {
                diagonal(I) += h(p,p);
                for (size_t q = 0; q < K; q++) {

                    if (p != q) {  // can't create/annihilate the same orbital twice
                        if (alpha_I.isOccupied(q)) {
                            diagonal(I) += 0.5 * g(p,p,q,q);
                            diagonal(I) -= 0.5 * g(p,q,q,p);
                        }
                    }

                    if (beta_I.isOccupied(q)) {
                        diagonal(I) += 0.5 * g(p,p,q,q);
                    }
                }  // loop over q
            }

            if (beta_I.isOccupied(p)) {
                diagonal(I) += h(p,p);
                for (size_t q = 0; q < K; q++) {

                    if (p != q) {  // can't create/annihilate the same orbital twice
                        if (beta_I.isOccupied(q)) {
                            diagonal(I) += 0.5 * g(p,p,q,q);
                            diagonal(I) -= 0.5 * g(p,q,q,p);
                        }
                    }

                    if (alpha_I.isOccupied(q)) {
                        diagonal(I) += 0.5 * g(p,p,q,q);
                    }
                }  // loop over q
            }
        }  // loop over q

    }  // alpha address (Ia) loop

    return diagonal;
}



}  // namespace GQCP
