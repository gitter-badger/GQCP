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
#include "QCMethod/CI/CI.hpp"

#include "ONVBasis/SeniorityZeroONVBasis.hpp"
#include "ONVBasis/SpinResolvedFrozenONVBasis.hpp"
#include "ONVBasis/SpinResolvedONVBasis.hpp"
#include "ONVBasis/SpinResolvedSelectedONVBasis.hpp"

#include <pybind11/pybind11.h>


namespace py = pybind11;


namespace gqcpy {


/**
 *  We want to provide an easier Python API for the construction of CI methods, e.g.
 *      gqcpy.CI(onv_basis)
 * 
 *  The following bindings implement this kind of factory behaviour.
 */

/**
 *  Bind a factory-like method for a CI method.
 * 
 *  @tparam ONVBasis            the type of the ONV basis associated to the CI method
 */
template <typename ONVBasis>
void bindCIFactoryMethod(py::module& module) {

    module.def("CI",
        [ ] (const ONVBasis& onv_basis, const size_t number_of_states = 1) {
            return GQCP::QCMethod::CI<ONVBasis>(onv_basis, number_of_states);
        },
        "Return an appropriate CI method.",
        py::arg("onv_basis"),
        py::arg("number_of_states") = 1
    );
}


/**
 *  Bind all types of CI methods to the gqcpy module.
 */
void bindCIFactory(py::module& module) {

    bindCIFactoryMethod<GQCP::SeniorityZeroONVBasis>(module);
    bindCIFactoryMethod<GQCP::SpinResolvedFrozenONVBasis>(module);
    bindCIFactoryMethod<GQCP::SpinResolvedONVBasis>(module);
    bindCIFactoryMethod<GQCP::SpinResolvedSelectedONVBasis>(module);
}


}  // namespace gqcpy
