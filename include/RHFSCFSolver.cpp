#include "RHFSCFSolver.hpp"


namespace GQCG {

/*
 *  CONSTRUCTORS
 */
RHFSCFSolver::RHFSCFSolver(GQCG::HamiltonianParameters ham_par, GQCG::Molecule molecule, double threshold, size_t maximum_number_of_iterations) :
    ham_par (ham_par),
    molecule (molecule),
    maximum_number_of_iterations (maximum_number_of_iterations),
    threshold (threshold)
{}



/*
 *  PUBLIC METHODS
 */

/**
 *  Solve the RHF SCF equations
 */
void RHFSCFSolver::solve() {

    // Obtain an initial guess for the AO density matrix by solving the generalized eigenvalue problem for H_core
    Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd> initial_generalized_eigensolver (this->ham_par.h.get_matrix_representation(), this->ham_par.S.get_matrix_representation());
    Eigen::MatrixXd C = initial_generalized_eigensolver.eigenvectors();

    Eigen::MatrixXd D_AO = GQCG::calculateRHFAO1RDM(C, this->molecule.N);


    size_t iteration_counter = 0;
    while (!(this->is_converged)) {
        Eigen::MatrixXd F_AO = this->calculateNewFockMatrix(D_AO);

        // Solve the generalized eigenvalue problem for the Fock matrix to get an improved density matrix
        Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd> generalized_eigensolver (F_AO, this->ham_par.S.get_matrix_representation());
        C = generalized_eigensolver.eigenvectors();

        Eigen::MatrixXd D_AO_previous = D_AO;  // store the previous density matrix to be able to check on convergence
        D_AO = GQCG::calculateRHFAO1RDM(C, this->molecule.N);


        // Check for convergence on the AO density matrix
        if ((D_AO - D_AO_previous).norm() <= this->threshold) {
            this->is_converged = true;

            // After the SCF procedure, we end up with canonical spatial orbitals, i.e. the Fock matrix should be diagonal in this basis
            // Let's check if this is the case, within double float precision
            GQCG::OneElectronOperator F (F_AO);
            F.transform(C);  // transform F to the MO basis with C
            assert(F.get_matrix_representation().isDiagonal());

            // Return the converged solution
            auto electronic_energy = GQCG::calculateRHFElectronicEnergy(D_AO, this->ham_par.h.get_matrix_representation(), F_AO);
            this->solution = GQCG::RHF(electronic_energy, C, generalized_eigensolver.eigenvalues());

        } else {  // not converged yet
            iteration_counter++;

            // If we reach more than this->maximum_number_of_iterations, the system is considered not to be converging
            if (iteration_counter >= this->maximum_number_of_iterations) {
                throw std::runtime_error("The SCF procedure did not converge.");
            }
        }
    }
}



}  // namespace GQCG