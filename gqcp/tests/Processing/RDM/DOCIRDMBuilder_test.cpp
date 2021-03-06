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
#define BOOST_TEST_MODULE "DOCI_RDM_test"

#include <boost/test/unit_test.hpp>

#include "Mathematical/Optimization/Eigenproblem/EigenproblemSolver.hpp"
#include "ONVBasis/SeniorityZeroONVBasis.hpp"
#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "Processing/RDM/DOCIRDMBuilder.hpp"
#include "Processing/RDM/SelectedRDMBuilder.hpp"
#include "QCMethod/CI/CI.hpp"
#include "QCMethod/CI/CIEnvironment.hpp"


/**
 *  Check if the 1- and 2-DMs for a seniority-zero ONV basis are equal to the 'selected' case.
 *  The system of interested is H2O//STO-3G, with 7 spatial orbitals and a Fock space dimension of 441.
 */
BOOST_AUTO_TEST_CASE ( specialized_vs_selected_DMs ) {

    // Set up the molecular Hamiltonian in a Löwdin-orthonormalized spinor basis.
    const auto molecule = GQCP::Molecule::ReadXYZ("data/h2o_Psi4_GAMESS.xyz");
    const auto N_P = molecule.numberOfElectrons() / 2;  // number of electron pairs

    GQCP::RSpinorBasis<double, GQCP::GTOShell> spinor_basis (molecule, "STO-3G");
    const auto K = spinor_basis.numberOfSpatialOrbitals();
    spinor_basis.lowdinOrthonormalize();

    const auto sq_hamiltonian = GQCP::SQHamiltonian<double>::Molecular(spinor_basis, molecule);


    // Do a dense DOCI calculation.
    const GQCP::SeniorityZeroONVBasis onv_basis {K, N_P};

    auto environment = GQCP::CIEnvironment::Dense(sq_hamiltonian, onv_basis);
    auto solver = GQCP::EigenproblemSolver::Dense();

    const auto linear_expansion = GQCP::QCMethod::CI<GQCP::SeniorityZeroONVBasis>(onv_basis).optimize(solver, environment).groundStateParameters();


    // Calculate the 1-DMs using specialized spin-resolved and 'selected' routines, and check if they are equal.
    const GQCP::DOCIRDMBuilder seniority_zero_rdm_builder {onv_basis};
    const auto one_rdms_specialized = seniority_zero_rdm_builder.calculate1RDMs(linear_expansion.coefficients());

    const GQCP::SpinResolvedSelectedONVBasis selected_onv_basis {onv_basis};
    const GQCP::SelectedRDMBuilder selected_rdm_builder {selected_onv_basis};
    const auto one_rdms_selected = selected_rdm_builder.calculate1RDMs(linear_expansion.coefficients());

    BOOST_CHECK(one_rdms_specialized.one_rdm.isApprox(one_rdms_selected.one_rdm, 1.0e-12));
    BOOST_CHECK(one_rdms_specialized.one_rdm_aa.isApprox(one_rdms_selected.one_rdm_aa, 1.0e-12));
    BOOST_CHECK(one_rdms_specialized.one_rdm_bb.isApprox(one_rdms_selected.one_rdm_bb, 1.0e-12));


    // Calculate the 2-DMs using specialized spin-resolved and 'selected' routines, and check if they are equal.
    const auto two_rdms_specialized = seniority_zero_rdm_builder.calculate2RDMs(linear_expansion.coefficients());
    const auto two_rdms_selected = selected_rdm_builder.calculate2RDMs(linear_expansion.coefficients());

    BOOST_CHECK(two_rdms_specialized.two_rdm_aaaa.isApprox(two_rdms_selected.two_rdm_aaaa, 1.0e-12));
    BOOST_CHECK(two_rdms_specialized.two_rdm_aabb.isApprox(two_rdms_selected.two_rdm_aabb, 1.0e-12));
    BOOST_CHECK(two_rdms_specialized.two_rdm_bbaa.isApprox(two_rdms_selected.two_rdm_bbaa, 1.0e-12));
    BOOST_CHECK(two_rdms_specialized.two_rdm_bbbb.isApprox(two_rdms_selected.two_rdm_bbbb, 1.0e-12));
    BOOST_CHECK(two_rdms_specialized.two_rdm.isApprox(two_rdms_selected.two_rdm, 1.0e-12));
}
