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
#include "Basis/ScalarBasis/GTOShell.hpp"
#include "Basis/transform.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;


namespace gqcpy {


void bindBasisTransform(py::module& module) {

    module.def("basisTransform",
        [ ] (GQCP::RSpinorBasis<double, GQCP::GTOShell>& spinor_basis, GQCP::SQHamiltonian<double>& sq_hamiltonian, const Eigen::MatrixXd& T) {

            GQCP::basisTransform(spinor_basis, sq_hamiltonian, GQCP::TransformationMatrix<double>{T});
        },
        py::arg("spinor_basis"),
        py::arg("sq_hamiltonian"),
        py::arg("T")
    );
}


}  // namespace gqcpy
