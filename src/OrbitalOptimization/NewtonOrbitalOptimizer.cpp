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
#include "OrbitalOptimization/NewtonOrbitalOptimizer.hpp"

#include "math/optimization/step.hpp"

#include "Eigen/Dense"
#include <unsupported/Eigen/MatrixFunctions>


namespace GQCP {


/*
 *  PUBLIC OVERRIDDEN METHODS
 */

/**
 *  Prepare this object (i.e. the context for the orbital optimization algorithm) to be able to check for convergence
 */
void NewtonOrbitalOptimizer::prepareConvergenceChecking(const HamiltonianParameters<double>& ham_par) {

    this->prepareOrbitalDerivativesCalculation(ham_par);

    // All Newton-based orbital optimizers need to calculate a gradient and Hessian
    this->gradient = this->calculateGradientVector(ham_par);
    this->hessian = this->calculateHessianMatrix(ham_par);
}


/**
 *  Determine if the algorithm has converged or not
 *  Specifically for the Newton-step based algorithms, this function
 *      - computes the gradient and checks its norm for convergence
 *      - if the gradient is zero, the Hessian is calculated and diagonalized and positive/negative definiteness is checked
 * 
 *  @param ham_par      the current Hamiltonian parameters
 * 
 *  @return if the algorithm is considered to be converged
 */
bool NewtonOrbitalOptimizer::checkForConvergence(const HamiltonianParameters<double>& ham_par) const {

    // Check for convergence on the norm
    if (this->gradient.norm() < this->oo_options.convergenceThreshold()) {
        if (this->newtonStepIsWellDefined()) {  // needs this->hessian
            return true;
        } else {
            return false;
        }
    }

    else {
        return false;
    }
}


/**
 *  Prepare this object (i.e. the context for the orbital optimization algorithm) to be able to calculate the new rotation matrix
 */
void NewtonOrbitalOptimizer::prepareRotationMatrixCalculation(const HamiltonianParameters<double>& ham_par) {

    // There are no special preparations to be done for the general Newton-based orbital optimizer
    this->prepareNewtonSpecificRotationMatrixCalculation(ham_par);
}


/**
 *  Produce a new rotation matrix by either
 *      - continuing in the direction of the largest (in absolute value) non-conforming eigenvalue (i.e. the smallest (negative) eigenvalue for minimization algorithms and the largest (positive) eigenvalue for maximization algorithms)
 *      - using the Newton step if it is well-defined
 * 
 *  @param ham_par      the current Hamiltonian parameters
 * 
 *  @return a unitary matrix that will be used to rotate the current Hamiltonian parameters into the next iteration
 */
SquareMatrix<double> NewtonOrbitalOptimizer::calculateNewRotationMatrix(const HamiltonianParameters<double>& ham_par) const {

    // The general goal of this function is to:
    //      1) determine the free orbital rotation generators, using gradient and Hessian information
    //      2) determine the full orbital rotation generators, by also including any redundant parameters
    //      3) calculate the unitary rotation matrix from the full orbital rotation generators

   const  auto full_kappa = this->calculateNewFullOrbitalGenerators(ham_par);  // should internally calculate the free orbital rotation generators

    return full_kappa.calculateRotationMatrix();  // matrix exponential
}



/* 
 *  PUBLIC METHODS
 */ 

/**
 *  @param ham_par      the current Hamiltonian parameters
 * 
 *  @return the current orbital gradient as a vector
 */
VectorX<double> NewtonOrbitalOptimizer::calculateGradientVector(const HamiltonianParameters<double>& ham_par) const {
    return this->calculateGradientMatrix(ham_par).strictLowerTriangle();
}


/**
 *  @param ham_par      the current Hamiltonian parameters
 * 
 *  @return the current orbital Hessian as a matrix
 */
SquareMatrix<double> NewtonOrbitalOptimizer::calculateHessianMatrix(const HamiltonianParameters<double>& ham_par) const {
    return this->calculateHessianTensor(ham_par).pairWiseStrictReduce();
}


/**
 *  @return if a Newton step would be well-defined (i.e. the Hessian is positive definite for minimizations and negative definite for maximizations)
 */
bool NewtonOrbitalOptimizer::newtonStepIsWellDefined() const {

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> hessian_diagonalizer (this->hessian);

    if (this->oo_options.shouldMinimize()) {

        // Can only produce a well-defined descending Newton step if the Hessian is positive definite
        if (hessian_diagonalizer.eigenvalues()(0) < 0) {
            return false;
        } else {
            return true;
        }
    }
    
    else {  // should maximize

        // Can only produce a well-defined ascending Newton step if the Hessian is negative definite
        const size_t dim = this->gradient.size();
        if (hessian_diagonalizer.eigenvalues()(dim - 1) > 0) {  // largest eigenvalue (they are sorted)
            return false;
        } else {
            return true;
        }
    }
}


/**
 *  If the Newton step is ill-defined, examine the Hessian and produce a new direction from it:
 *      - for minimization algorithms, this is the eigenvector that corresponds to the smallest (negative) eigenvalue of the Hessian
 *      - for maximization algorithms, this is the eigenvector that corresponds to the largest (positive) eigenvalue of the Hessian
 * 
 *  @return the new direction from the Hessian if the Newton step is ill-defined
 */
VectorX<double> NewtonOrbitalOptimizer::directionFromHessian() const {
    
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> hessian_diagonalizer (this->hessian);

    if (this->oo_options.shouldMinimize()) {
        if (hessian_diagonalizer.eigenvalues()(0) < 0) {
            return hessian_diagonalizer.eigenvectors().col(0);
        }
    }
    
    else {  // should maximize
        const size_t dim = this->gradient.size();
        if (hessian_diagonalizer.eigenvalues()(dim - 1) > 0) {  // largest eigenvalue (they are sorted)
            return hessian_diagonalizer.eigenvectors().col(dim - 1);
        }
    }

    // We shouldn't ever reach these lines, unless this function was wrongly called
    throw std::invalid_argument("NewtonOrbitalOptimizer::directionFromHessian(): A direction from the Hessian was tried to be generated but the Newton step is well-defined.");
}


/**
 *  Use gradient and Hessian information to determine a new direction for the 'free' orbital rotation generators kappa. Note that a distinction is made between 'free' generators, i.e. those that are calculated from the gradient and Hessian information and the 'full' generators, which also include the redundant parameters (that can be set to zero). The 'full' generators are used to calculate the total rotation matrix using the matrix exponential
 * 
 *  @param ham_par      the current Hamiltonian parameters
 * 
 *  @return the new free orbital generators
 */
OrbitalRotationGenerators NewtonOrbitalOptimizer::calculateNewFreeOrbitalGenerators(const HamiltonianParameters<double>& ham_par) const {

    // If the norm hasn't converged, continue in the Newton direction
    // Until we have completely read Nocedal & Wright, this is the way we're doing Newton optimization
    if (this->gradient.norm() > this->oo_options.convergenceThreshold()) {

        const size_t dim = this->gradient.size();
        const VectorFunction gradient_function = [this] (const VectorX<double>& x) { return this->gradient; };
        const MatrixFunction hessian_function = [this] (const VectorX<double>& x) { return this->hessian; };

        return OrbitalRotationGenerators(newtonStep(VectorX<double>::Zero(dim), gradient_function, hessian_function));  // with only the free parameters
    }

    else {  // the gradient has converged but the Hessian is indefinite
        // We're sure that if the program reaches this step, the Newton step is ill-defined
        return OrbitalRotationGenerators(this->directionFromHessian());
    }
}


}  // namespace GQCP
