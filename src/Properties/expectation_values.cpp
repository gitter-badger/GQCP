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
#include "Properties/expectation_values.hpp"


namespace GQCP {


/*
 *  MIXED OPERATORS
 */

/**
 *  @param ham_par      the electronic Hamiltonian containing one- and two-electron operators
 *  @param one_rdm      the 1-RDM
 *  @param two_rdm      the 2-RDM
 *
 *  @return the expectation value of the given electronic Hamiltonian
 */
double calculateExpectationValue(const SQHamiltonian<double>& ham_par, const OneRDM<double>& one_rdm, const TwoRDM<double>& two_rdm) {

    return calculateExpectationValue(ham_par.core(), one_rdm)[0] + calculateExpectationValue(ham_par.twoElectron(), two_rdm)[0];  // SQHamiltonian contains ScalarSQOperators, so we access with [0]
}


}  // namespace GQCP
