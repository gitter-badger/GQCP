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


#include "Mathematical/Representation/Matrix.hpp"


namespace GQCP {


/**
 *  A container class to store an eigenpair, i.e. an eigenvector with a corresponding eigenvalue
 */
class Eigenpair {
private:
    double eigenvalue;
    VectorX<double> eigenvector;


public:
    // CONSTRUCTORS
    /**
     *  A constructor that sets the eigenvalue to zero and the corresponding eigenvector to zeros
     *
     *  @param dimension        the dimension of the eigenvector
     */
    explicit Eigenpair(size_t dimension = 1);

    /**
     *  @param eigenvalue       the eigenvalue
     *  @param eigenvector      the eigenvector
     */
    Eigenpair(double eigenvalue, const VectorX<double>& eigenvector);


    // GETTERS
    double get_eigenvalue() const { return this->eigenvalue; };
    const VectorX<double>& get_eigenvector() const { return this->eigenvector; };


    // PUBLIC METHODS
    /**
     *  @param other            the other Eigenpair
     *  @param tolerance        a tolerance for comparison
     *
     *  @return if this Eigenpair is equal to the other: if the eigenvalues and eigenvectors are equal given the tolerance
     */
    bool isEqual(const Eigenpair& other, double tolerance=1.0e-08) const;
};


}  // namespace GQCP
