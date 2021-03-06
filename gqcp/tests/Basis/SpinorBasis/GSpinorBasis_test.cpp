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
#define BOOST_TEST_MODULE "GSpinorBasis_test"

#include <boost/test/unit_test.hpp>

#include "Basis/SpinorBasis/GSpinorBasis.hpp"

#include "Basis/ScalarBasis/GTOShell.hpp"
#include "Molecule/Molecule.hpp"


/**
 *  Test if the most elementary constructor throws errors when expected.
 */
BOOST_AUTO_TEST_CASE ( constructor_throws ) {

    // Initialize two scalar bases, one for the alpha component and one for the beta component.
    const auto molecule = GQCP::Molecule::ReadXYZ("data/h2.xyz");

    const GQCP::ScalarBasis<GQCP::GTOShell> alpha_scalar_basis (molecule, "STO-3G");
    const auto K_alpha = alpha_scalar_basis.numberOfBasisFunctions();  // 2

    const GQCP::ScalarBasis<GQCP::GTOShell> beta_scalar_basis (molecule, "6-31G");
    const auto K_beta = beta_scalar_basis.numberOfBasisFunctions();  // 4


    // Initialize two coefficient matrices, one with compatible and one with incompatible dimensions.
    const GQCP::TransformationMatrix<double> T_compatible (K_alpha + K_beta);
    const GQCP::TransformationMatrix<double> T_incompatible (K_alpha + K_beta - 1);


    // Check if the constructor throws upon receiving incompatible arguments.
    using SpinorBasisType = GQCP::GSpinorBasis<double, GQCP::GTOShell>;  // needed to resolve compilation errors with boost::test
    BOOST_CHECK_THROW(SpinorBasisType spinor_basis (alpha_scalar_basis, beta_scalar_basis, T_incompatible), std::invalid_argument);
    BOOST_CHECK_NO_THROW(SpinorBasisType spinor_basis (alpha_scalar_basis, beta_scalar_basis, T_compatible));
}


/**
 *  Test some basic functionality: number of spinors, number of alpha coefficients, number of beta coefficients.
 */
BOOST_AUTO_TEST_CASE ( basic_functionality ) {

    // Initialize a spinor basis.
    const auto molecule = GQCP::Molecule::ReadXYZ("data/h2.xyz");
    const GQCP::GSpinorBasis<double, GQCP::GTOShell> spinor_basis (molecule, "STO-3G");


    // Check some basic functionality.
    BOOST_CHECK(spinor_basis.numberOfCoefficients(GQCP::SpinComponent::ALPHA) == 2);
    BOOST_CHECK(spinor_basis.numberOfCoefficients(GQCP::SpinComponent::BETA) == 2);
    BOOST_CHECK(spinor_basis.numberOfSpinors() == 4);
}


/**
 *  Check if the alpha- and beta-coefficient matrices together form the total coefficient matrix.
 */
BOOST_AUTO_TEST_CASE ( alpha_beta_coefficient_matrix ) {

    // Initialize a spinor basis with a different scalar basis for both the components.
    const auto molecule = GQCP::Molecule::ReadXYZ("data/h2.xyz");
    const GQCP::GSpinorBasis<double, GQCP::GTOShell> spinor_basis (molecule, "STO-3G", "6-31G");
    const auto K_alpha = spinor_basis.numberOfCoefficients(GQCP::SpinComponent::ALPHA);
    const auto K_beta = spinor_basis.numberOfCoefficients(GQCP::SpinComponent::BETA);
    const auto M = K_alpha + K_beta;  // number of spinors


    // Initialize reference values for the alpha, beta and total coefficient matrix and check the results.
    GQCP::MatrixX<double> C_alpha_ref = GQCP::MatrixX<double>::Zero(K_alpha, M);
    C_alpha_ref.topLeftCorner(K_alpha, K_alpha) = GQCP::MatrixX<double>::Identity(K_alpha, K_alpha);

    GQCP::MatrixX<double> C_beta_ref = GQCP::MatrixX<double>::Zero(K_beta, M);
    C_beta_ref.bottomRightCorner(K_beta, K_beta) = GQCP::MatrixX<double>::Identity(K_beta, K_beta);

    GQCP::SquareMatrix<double> C_ref = GQCP::SquareMatrix<double>::Identity(M, M);

    BOOST_CHECK(spinor_basis.coefficientMatrix(GQCP::SpinComponent::ALPHA).isApprox(C_alpha_ref, 1.0e-08));
    BOOST_CHECK(spinor_basis.coefficientMatrix(GQCP::SpinComponent::BETA).isApprox(C_beta_ref, 1.0e-08));
    BOOST_CHECK(spinor_basis.coefficientMatrix().isApprox(C_ref, 1.0e-08));
}


/**
 *  Check if the quantization of the Coulomb operator yields zero elements where expected.
 * 
 *  For a test system of H2//STO-3G, we expect zero blocks whenever the indices P,Q and R,S do not belong to the same alpha or beta set.
 */
BOOST_AUTO_TEST_CASE ( Coulomb_quantization_zero_blocks ) {

    // Set up a small general spinor basis.
    const auto molecule = GQCP::Molecule::ReadXYZ("data/h2.xyz");
    const GQCP::GSpinorBasis<double, GQCP::GTOShell> spinor_basis (molecule, "STO-3G");


    // Quantize the Coulomb operator and check some ought-to-be-zero elements.
    const auto g = spinor_basis.quantize(GQCP::Operator::Coulomb()).parameters();

    /**
     *  For this system, K_alpha = 2 and K_beta = 2, so we expect:
     *      - g(0,2,0,0) to be zero: (P=0, Q=2) do not both belong to the alpha indices;
     *      - g(2,3,1,3) to be zero: (R=1, S=3) do not both belong to the beta indices.
     */
    BOOST_CHECK(std::abs(g(0,2,0,0)) < 1.0e-12);
    BOOST_CHECK(std::abs(g(2,3,1,3)) < 1.0e-12);
}
