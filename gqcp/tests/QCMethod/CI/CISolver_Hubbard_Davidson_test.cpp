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
#define BOOST_TEST_MODULE "DavidsonHubbardSolver"

#include <boost/test/unit_test.hpp>

#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "QCMethod/CI/CISolver.hpp"
#include "QCMethod/CI/HamiltonianBuilder/FCI.hpp"
#include "QCMethod/CI/HamiltonianBuilder/Hubbard.hpp"


BOOST_AUTO_TEST_CASE ( test_Hubbard_vs_FCI_davidson ) {

    // Check if FCI and Hubbard produce the same results for Hubbard Hamiltonian

    // Create the Hamiltonian for a random Hubbard hopping matrix
    size_t K = 4;
    auto H = GQCP::HoppingMatrix::Random(K);
    auto sq_hamiltonian = GQCP::SQHamiltonian<double>::Hubbard(H);


    // Create the Hubbard and FCI modules
    size_t N = 2;
    GQCP::SpinResolvedONVBasis fock_space (K, N, N);  // dim = 36
    GQCP::Hubbard hubbard (fock_space);
    GQCP::FCI fci (fock_space);

    GQCP::CISolver hubbard_solver (hubbard, sq_hamiltonian);
    GQCP::CISolver fci_solver (fci, sq_hamiltonian);


    // Solve with Davidson
    GQCP::VectorX<double> initial_guess = fock_space.randomExpansion();
    GQCP::DavidsonSolverOptions solver_options (initial_guess);
    hubbard_solver.solve(solver_options);
    fci_solver.solve(solver_options);

    auto fci_energy = fci_solver.get_eigenpair().get_eigenvalue();
    auto hubbard_energy = hubbard_solver.get_eigenpair().get_eigenvalue();

    BOOST_CHECK(std::abs(fci_energy - (hubbard_energy)) < 1.0e-06);
}


BOOST_AUTO_TEST_CASE ( test_Hubbard_vs_FCI_davidson_large ) {

    // Check if FCI and Hubbard produce the same results for Hubbard Hamiltonian

    // Create the Hamiltonian for a random Hubbard hopping matrix
    size_t K = 6;
    auto H = GQCP::HoppingMatrix::Random(K);
    auto sq_hamiltonian = GQCP::SQHamiltonian<double>::Hubbard(H);


    // Create the Hubbard and FCI modules
    size_t N = 3;
    GQCP::SpinResolvedONVBasis fock_space (K, N, N);  // dim = 400
    GQCP::Hubbard hubbard (fock_space);
    GQCP::FCI fci (fock_space);

    GQCP::CISolver hubbard_solver (hubbard, sq_hamiltonian);
    GQCP::CISolver fci_solver (fci, sq_hamiltonian);


    // Solve with Davidson
    GQCP::VectorX<double> initial_guess = fock_space.randomExpansion();
    GQCP::DavidsonSolverOptions solver_options (initial_guess);
    hubbard_solver.solve(solver_options);
    fci_solver.solve(solver_options);

    auto fci_energy = fci_solver.get_eigenpair().get_eigenvalue();
    auto hubbard_energy = hubbard_solver.get_eigenpair().get_eigenvalue();

    BOOST_CHECK(std::abs(fci_energy - (hubbard_energy)) < 1.0e-06);
}