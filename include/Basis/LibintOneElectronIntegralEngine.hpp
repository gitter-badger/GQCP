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
#ifndef GQCP_LIBINTONEELECTRONINTEGRALENGINE_HPP
#define GQCP_LIBINTONEELECTRONINTEGRALENGINE_HPP


#include "Basis/BaseOneElectronIntegralEngine.hpp"
#include "Basis/GTOShell.hpp"
#include "Basis/LibintInterfacer.hpp"
#include "Operator/FirstQuantized/Operator.hpp"



namespace GQCP {


/**
 *  A one-electron integral engine that uses Libint2 as its backend
 */
template <size_t _N, typename _Scalar>
class LibintOneElectronIntegralEngine : public BaseOneElectronIntegralEngine<GTOShell, _N, _Scalar> {
private:
    libint2::Engine libint2_engine;


public:
    // CONSTRUCTORS
    LibintOneElectronIntegralEngine(const OverlapOperator& op) :
        libint2_engine ()

};


}  // namespace GQCP



#endif  // GQCP_LIBINTONEELECTRONINTEGRALENGINE_HPP
