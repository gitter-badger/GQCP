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


#include "ONVBasis/SeniorityZeroONVBasis.hpp"
#include "Processing/RDM/RDMs.hpp"


namespace GQCP {


/**
 *  A class capable of calculating 1- and 2-DMs from doubly-occupied ONV bases.
 */
class DOCIRDMBuilder {
    SeniorityZeroONVBasis onv_basis;


public:
    // CONSTRUCTORS

    /**
     *  @param onv_basis            the seniority-zero ONV basis
     */
    explicit DOCIRDMBuilder(const SeniorityZeroONVBasis& onv_basis);



    // PUBLIC METHODS

    /**
     *  @param x        the coefficient vector representing the DOCI wave function
     *
     *  @return the 1-RDMs given a coefficient vector
     */
    OneRDMs<double> calculate1RDMs(const VectorX<double>& x) const;

    /**
     *  @param x        the coefficient vector representing the DOCI wave function
     *
     *  @return the 2-RDMs given a coefficient vector
     */
    TwoRDMs<double> calculate2RDMs(const VectorX<double>& x) const;
};


}  // namespace GQCP
