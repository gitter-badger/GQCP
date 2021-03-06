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
#include "Mathematical/Representation/Tensor.hpp"

#include <iostream>


namespace GQCP {


/**
 *  A square extension of a rank-4 tensor
 *
 *  @tparam _Scalar      the scalar type
 */
template<typename _Scalar>
class SquareRankFourTensor: public Tensor<_Scalar, 4> {
public:
    using Scalar = _Scalar;

    using Base = Tensor<Scalar, 4>;
    using Self = SquareRankFourTensor<Scalar>;


public:

    /*
     *  CONSTRUCTORS
     */

    /**
     *  Default constructor
     */
    SquareRankFourTensor() : Base() {}


    /**
     *  Construct a square rank-4 tensor given a dimension
     *
     *  @param dim      the dimension of the rank-4 tensor
     */
    SquareRankFourTensor(size_t dim) :
        Base(dim, dim, dim, dim)
    {}


    /**
     *  A basic constructor from a rank-4 GQCP::Tensor that checks if the given tensor is square
     *
     *  @param tensor       the tensor that should be square
     */
    SquareRankFourTensor(const Base& tensor) :
        Base(tensor)
    {
        // Check if the given tensor is square
        auto dims = this->dimensions();
        if ((dims[0] != dims[1]) || (dims[1] != dims[2]) || (dims[2] != dims[3]) ) {
            throw std::invalid_argument("SquareRankFourTensor(Eigen::TensorBase<OtherDerived, AccessLevel>): The given tensor should have equal dimensions in every rank.");
        }
    }


    /**
     *  Constructor from Eigen::Tensor expressions
     *
     *  @tparam ExpDerived      the type of the Eigen expression (normally generated by the compiler)
     *  @tparam AccessLevel     the access level of the TensorBase (normally generated by the compiler)
     *
     *  @param exp              the Eigen::Tensor expression
     */
    template <typename ExpDerived, int AccessLevel>
    SquareRankFourTensor(const Eigen::TensorBase<ExpDerived, AccessLevel>& exp) :
        Self(Base(exp))  // the Base constructor returns the required type for the square-checking constructor
    {}



    /*
     *  NAMED CONSTRUCTORS
     */

    /**
     *  Read a square rank-4 tensor from a given file
     *
     *  @param filename     the name of the file to be read in
     *  @param dim          the dimension of the square rank-4 tensor
     */
    static Self FromFile(const std::string& filename, size_t dim) {

        Self result (dim);
        result.setZero();  // make sure that the tensor is initialized to zero values before reading in

        std::ifstream file (filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::vector<std::string> splitted_line;  // create a container for the line to be split in

                // Split the line on any whitespace or tabs.
                boost::split(splitted_line, line, boost::is_any_of(" \t"), boost::token_compress_on);

                if (splitted_line.size() != 5) {
                    throw std::runtime_error("SquareRankFourTensor::FromFile(std::string, size_t): Found a line that doesn't contain exactly 5 fields delimited by whitespace.");
                }

                auto i = std::stoi(splitted_line[0]);
                auto j = std::stoi(splitted_line[1]);
                auto k = std::stoi(splitted_line[2]);
                auto l = std::stoi(splitted_line[3]);
                auto value = std::stod(splitted_line[4]);

                result(i,j,k,l) = value;
            }

            file.close();
        } else {
            throw std::runtime_error("SquareRankFourTensor::FromFile(std::string, size_t): Cannot open the given file. Maybe you specified a wrong path?");
        }

        return result;
    }


    /*
     *  OPERATORS
     */

    /**
     *  Assignment operator from Eigen::Tensor expressions
     *
     *  @tparam ExpDerived      the type of the Eigen expression (normally generated by the compiler)
     *  @tparam AccessLevel     the access level of the TensorBase (normally generated by the compiler)
     *
     *  @param exp              the Eigen::Tensor expression
     */
    template <typename ExpDerived, int AccessLevel>
    Self& operator=(const Eigen::TensorBase<ExpDerived, AccessLevel>& exp) {
        this->Base::operator=(exp);
        return (*this);
    }



    /*
     *  GETTERS
     */

    size_t get_dim() const { return this->dimension(0); }  // all tensor dimensions are equal because of the constructor



    /*
     *  PUBLIC METHODS
     */
   
    /**
     *  @return the pair-wise reduction of this square rank-4 tensor, i.e. the tensor analog of a strict "lower triangle" as a matrix in column major form
     *
     *  The matrix indices (m,n) come from the tensor indices (i,j,k,l) and are such that:
     *      - m is compounded in a column major way from i and j, with the restriction i>j
     *      - n is compounded in a column major way from k and l, with the restriction k>l
     */
    SquareMatrix<double> pairWiseStrictReduce() const {

        // Initialize the resulting matrix
        const auto K = this->get_dim();
        SquareMatrix<double> M = SquareMatrix<double>::Zero(K*(K-1)/2, K*(K-1)/2);

        // Calculate the compound indices and bring the elements from the tensor over into the matrix
        size_t row_index = 0;
        for (size_t j = 0; j < K; j++) {  // "column major" ordering for row_index<-i,j so we do j first, then i
            for (size_t i = j+1; i < K; i++) {  // in column major indices, columns are contiguous, so the first of two indices changes more rapidly
                // require i > j for "lower triangle"

                size_t column_index = 0;
                for (size_t l = 0; l < K; l++) {  // "column major" ordering for column_index<-k,l so we do l first, then k
                    for (size_t k = l+1; k < K; k++) {  // in column major indices, columns are contiguous, so the first of two indices changes more rapidly
                        // require l > k for "lower triangle"

                        M(row_index,column_index) = this->operator()(i,j,k,l);

                        column_index++;
                    }
                }

                row_index++;
            }
        }

        return M;
    }


    /**
     *  In-place contract of a given matrix with this square rank-4 tensor
     *
     *  @tparam TransformationScalar        the type of scalar used for the transformation matrix
     *
     *  @param M                            the contraction matrix: contraction performed with the first index axis
     *  @param tensor_index                 the index axis which is contracted
     *
     *    e.g.
     *     if index 0 (1-st) is chosen:
     *      T_transformed (i, j , k, l) = Sum(q) T (q j k l) * A (q, i)
     *     if index 2 (3-rd) is chosen:
     *      T_transformed (i, j , k, l) = Sum(q) T (i j q l) * A (q, k)
     *
     */
    template <typename MultiplicationScalar = Scalar>
    void matrixContraction(const SquareMatrix<MultiplicationScalar>& M, size_t index) {

        if (index >= 4) {
            throw std::invalid_argument("SquareRankFourTensor::matrixContraction(SquareMatrix<MultiplicationScalar>, size_t): The selected index should be smaller than the rank of the tensor.");
        }

        Eigen::array<Eigen::IndexPair<int>, 1> contraction_pair = {Eigen::IndexPair<int>(0, index)};

        // Eigen3 does not accept a way to specify the output axes: instead, it retains the order from left to right of the axes that survive the contraction.
        // This means that, in order to get the right ordering of the axes, we will have to swap axes

        std::array<int, 4> shuffle {1,2,3,0};
        shuffle[index] = 0;
        for (int i = index+1; i < 4; i++ ) {
            shuffle[i] = i;
        }

        // Since we're only getting M as a matrix, we should make the appropriate tensor to perform contractions
        // For the const argument, we need the const in the template
        //      For more info, see: https://stackoverflow.com/questions/45283468/eigen-const-tensormap
        Eigen::TensorMap<Eigen::Tensor<const MultiplicationScalar, 2>> M_tensor (M.data(), M.rows(), M.cols());

        Self T_transformed = M_tensor.contract(this->Eigen(), contraction_pair).shuffle(shuffle);

        *this = T_transformed;
    }
};


}  // namespace GQCP
