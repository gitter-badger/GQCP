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


#include "Mathematical/Representation/SquareMatrix.hpp"


namespace GQCP {


/**
 *  The Hubbard hopping matrix.
 * 
 *  @tparam _Scalar         the scalar type of the elements
 */
template <typename _Scalar>
class HoppingMatrix:
    public SquareMatrix<_Scalar> {

public:
    using Scalar = _Scalar;


public:

    /*
     *  CONSTRUCTORS
     */

    /**
     *  A default constructor.
     */
    HoppingMatrix() : SquareMatrix<Scalar>() {}


    /**
     *  A constructor required for compatibility with Pybind11. In its 'Eigen' bindings (eigen.h), it makes a call "Type(fits.rows, fits.cols)". This constructor should be called there.
     */
    HoppingMatrix(const size_t cols, const size_t rows) :
        SquareMatrix<Scalar>(MatrixX<Scalar>(cols, rows))
    {}


    /**
     *  @param H        the Hubbard hopping matrix
     */
    HoppingMatrix(const SquareMatrix<Scalar>& H) :
        SquareMatrix<Scalar>(H)
    {
        if (!H.adjoint().isApprox(H)) {
            throw std::invalid_argument("HoppingMatrix::HoppingMatrix(const SquareMatrix<Scalar>&): The given hopping matrix must be Hermitian.");
        }
    }


    /**
     *  Generate the Hubbard hopping matrix from an adjacency matrix and Hubbard model parameters U and t.
     *
     *  @param A        the Hubbard adjacency matrix, specifying the connectivity of the Hubbard lattice
     *  @param t        the Hubbard parameter t. Note that a positive value for t means a negative neighbour hopping term
     *  @param U        the Hubbard parameter U
     *
     *  @note This constructor is only available in the real case (for the std::enable_if, see https://stackoverflow.com/a/17842695/7930415).
     */
    template <typename Z = Scalar>
    HoppingMatrix(const SquareMatrix<double>& A, const double t, const double U,
                  typename std::enable_if<std::is_same<Z, double>::value>::type* = 0) :
        HoppingMatrix(U * SquareMatrix<double>::Identity(A.dimension(), A.dimension()) - t * A)
    {}


    /*
     *  NAMED CONSTRUCTORS
     */

    /**
     *  @param csline           a comma-separated line that contains the upper triangle (in column-major ordering) of the Hubbard hopping matrix
     * 
     *  @return the hopping matrix that corresponds to the given comma-separated line
     */
    template <typename Z = Scalar>
    static enable_if_t<std::is_same<Z, double>::value, HoppingMatrix<double>> FromCSLine(const std::string& csline) {

        if (csline.empty()) {
            throw std::invalid_argument("HoppingMatrix::FromCSLine(const std::string&): Comma-separated line was empty!");
        }

        // Split the comma-separated line into a std::vector
        std::vector<std::string> splitted_line;
        boost::split(splitted_line, csline, boost::is_any_of(","));

        std::vector<double> triagonal_data;
        for (const auto& x : splitted_line) {
            triagonal_data.push_back(std::stod(x));  // immediately convert string to double
        }

        // Map the std::vector<double> into a VectorX<double> to be used into an other constructor
        GQCP::VectorX<double> upper_triangle = Eigen::Map<Eigen::VectorXd>(triagonal_data.data(), triagonal_data.size());
        return GQCP::HoppingMatrix<double>::SymmetricFromUpperTriangle(upper_triangle);
    }


    /**
     *  @param K        the number of lattice sites
     *
     *  @return a random hopping matrix with elements distributed uniformly in [-1.0, 1.0]
     * 
     *  @note This method is only available for real scalars.
     */
    template <typename Z = Scalar>
    static enable_if_t<std::is_same<Z, double>::value, HoppingMatrix<double>> Random(const size_t K) {

        VectorX<double> v = VectorX<double>::Random(K*(K+1)/2);  // random free variables

        return HoppingMatrix<double>::SymmetricFromUpperTriangle(v);
    }


    /*
     *  PUBLIC METHODS
     */

    /**
     *  @return the number of lattice sites corresponding used in this hopping matrix
     */
    size_t numberOfLatticeSites() const { return this->dimension(); }
};


}  // namespace GQCP
