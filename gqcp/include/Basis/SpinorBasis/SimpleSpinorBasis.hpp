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


#include "Basis/TransformationMatrix.hpp"
#include "Utilities/CRTP.hpp"
#include "Utilities/type_traits.hpp"


namespace GQCP {


/**
 *  A class that represents a spinor basis that has no internal structure (hence 'simple'). It is used as a CRTP base class for restricted and generalized spinor bases because they admit common functionality.
 * 
 * 
 *  @tparam _ExpansionScalar            the scalar type of the expansion coefficients
 *  @tparam _DerivedSpinorBasis         the CRTP template argument of the derived spinor basis
 */
template <typename _ExpansionScalar, typename _DerivedSpinorBasis>
class SimpleSpinorBasis:
    public CRTP<_DerivedSpinorBasis> {

public:
    using ExpansionScalar = _ExpansionScalar;


protected:
    TransformationMatrix<ExpansionScalar> C;  // the matrix that holds the the expansion coefficients, i.e. that expresses the spinors in terms of the underlying scalar basis/bases


public:

    /*
     *  CONSTRUCTORS
     */

    /**
     *  @param C                    the matrix that holds the the expansion coefficients, i.e. that expresses the spinors in terms of the underlying scalar basis
     */
    SimpleSpinorBasis(const TransformationMatrix<ExpansionScalar>& C) :
        C (C)
    {}



    /* 
     *  PUBLIC METHODS
     */

    /**
     *  @return the transformation matrix between the scalar basis and the current orbitals
     */
    const TransformationMatrix<ExpansionScalar>& coefficientMatrix() const { return this->C; }

    /**
     *  @param precision                the precision used to test orthonormality
     * 
     *  @return if this spinor basis is orthonormal within the given precision
     */
    bool isOrthonormal(const double precision = 1.0e-08) const {

        const auto S = this->overlap().parameters();

        const auto dim = this->simpleDimension();
        return S.isApprox(SquareMatrix<ExpansionScalar>::Identity(dim, dim), precision);
    }


    /**
     *  @return the transformation matrix to the Löwdin basis: T = S_current^{-1/2}
     */
    TransformationMatrix<double> lowdinOrthonormalizationMatrix() const {

        // Calculate S^{-1/2}, where S is epxressed in the current spinor basis
        const auto S = this->overlap().parameters();
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> saes (S);
        return TransformationMatrix<double>(saes.operatorInverseSqrt());
    }


    /**
     *  Transform the spinor basis to the 'Löwdin basis', which is the orthonormal basis that we transform to with T = S^{-1/2}, where S is the current overlap matrix.
     */
    void lowdinOrthonormalize() {

        this->C = this->lowdinOrthonormalizationMatrix();
    }


    /**
     *  @return the overlap (one-electron) operator of this restricted spinor basis
     */
    ScalarSQOneElectronOperator<ExpansionScalar> overlap() const {

        return this->derived().quantize(Operator::Overlap());
    }


    /**
     *  Rotate the spinor basis to another one using the given unitary transformation matrix
     * 
     *  @param U            the unitary transformation matrix that transforms both the alpha- and beta components
     */
    void rotate(const TransformationMatrix<ExpansionScalar>& U) {

        // Check if the given matrix is actually unitary
        if (!U.isUnitary(1.0e-12)) {
            throw std::invalid_argument("SimpleSpinorBasis::rotate(const TransformationMatrix<ExpansionScalar>&): The given transformation matrix is not unitary.");
        }

        this->transform(U);
    }


    /**
     *  Rotate the spinor basis to another one using the unitary transformation matrix that corresponds to the given Jacobi rotation parameters
     * 
     *  @param jacobi_rotation_parameters       the Jacobi rotation parameters (p, q, angle) that are used to specify a Jacobi rotation: we use the (cos, sin, -sin, cos) definition for the Jacobi rotation matrix
     * 
     *  @note this function is only available for real spinor bases because Jacobi rotation parameters generate real rotations
     */
    template<typename Z = ExpansionScalar>
    enable_if_t<std::is_same<Z, double>::value> rotate(const JacobiRotationParameters& jacobi_rotation_parameters) {

        const auto dim = this->simpleDimension();
        const auto J = TransformationMatrix<double>::FromJacobi(jacobi_rotation_parameters, dim);
        this->rotate(J);
    }


    /**
     *  @return the dimension of this simple spinor basis, i.e. the dimension of the underlying coefficient matrix
     */
    size_t simpleDimension() const { return this->C.cols(); }

    /**
     *  Transform the spinor basis another one using the given transformation matrix
     * 
     *  @param T            the transformation matrix that transforms both the alpha- and beta components
     */
    void transform(const TransformationMatrix<ExpansionScalar>& T) {

        this->C.transform(T);
    }
};


}  // namespace GQCP
