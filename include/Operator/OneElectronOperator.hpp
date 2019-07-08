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
#ifndef GQCP_ONEELECTRONOPERATOR_HPP
#define GQCP_ONEELECTRONOPERATOR_HPP

#include "Mathematical/ChemicalMatrix.hpp"
#include "Operator/BaseOperator.hpp"

#include "Mathematical/ScalarFunction.hpp"
#include "OrbitalOptimization/JacobiRotationParameters.hpp"



namespace GQCP {


/**
 *  A class that represents a one-electron operator in an orbital basis
 *
 *  @tparam _Scalar      the scalar type
 */
template <typename _Scalar>
class OneElectronOperator : public ChemicalMatrix<_Scalar>, public BaseOperator<OneElectronOperator<_Scalar>> {
public:

    using Scalar = _Scalar;

    using BaseRepresentation = ChemicalMatrix<Scalar>;
    using Self = OneElectronOperator<Scalar>;


public:

    /*
     *  CONSTRUCTORS
     */

    using ChemicalMatrix<Scalar>::ChemicalMatrix;  // use base constructors


    /*
     *  PUBLIC METHODS
     */


    using BaseOperator<OneElectronOperator<Scalar>>::rotate;  // bring over rotate from the base class


    /**
     *  In-place rotate the matrix representation of the one-electron operator using a unitary Jacobi rotation matrix constructed from the Jacobi rotation parameters. Note that this function is only available for real (double) matrix representations
     *
     *  @param jacobi_rotation_parameters       the Jacobi rotation parameters (p, q, angle) that are used to specify a Jacobi rotation: we use the (cos, sin, -sin, cos) definition for the Jacobi rotation matrix. See transform() for how the transformation matrix between the two bases should be represented
     */
    template<typename Z = Scalar>
    enable_if_t<std::is_same<Z, double>::value> rotate(const JacobiRotationParameters& jacobi_rotation_parameters) {

        auto p = jacobi_rotation_parameters.get_p();
        auto q = jacobi_rotation_parameters.get_q();
        auto angle = jacobi_rotation_parameters.get_angle();

        double c = std::cos(angle);
        double s = std::sin(angle);


        // Use Eigen's Jacobi module to apply the Jacobi rotations directly (cfr. T.adjoint() * M * T)
        Eigen::JacobiRotation<double> jacobi (c, s);

        this->applyOnTheLeft(p, q, jacobi.adjoint());
        this->applyOnTheRight(p, q, jacobi);
    }


    /**
     *  @param x        the vector/point at which the scalar functions should be evaluated
     *
     *  @return a one-electron operator corresponding to the evaluated scalar functions
     *
     *  Note that this function is only available for OneElectronOperators whose Scalar is a derived class of ScalarFunction
     */
    template <typename Z = Scalar>
    enable_if_t<std::is_base_of<ScalarFunction<typename Z::Valued, typename Z::Scalar, Z::Cols>, Z>::value,
    OneElectronOperator<typename Z::Valued>> evaluate(const Vector<typename Z::Scalar, Z::Cols>& x) const {

        Eigen::Matrix<typename Z::Valued, ChemicalMatrix<Z>::Rows, ChemicalMatrix<Z>::Cols> result (this->rows(), this->cols());
        auto result_op = ChemicalMatrix<typename Z::Valued>(result);

        for (size_t i = 0; i < this->rows(); i++) {
            for (size_t j = 0; j < this->cols(); j++) {
                result_op(i,j) = (*this)(i,j).operator()(x);
            }
        }
        return result_op;
    }
};



}  // namespace GQCP



#endif  // GQCP_ONEELECTRONOPERATOR_HPP
