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
#include "QCModel/Geminals/vAP1roG.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;


namespace gqcpy {


void bindQCModelvAP1roG(py::module& module) {
    py::class_<GQCP::QCModel::vAP1roG>(module, "QCModel_vAP1roG", "The variationally optimized AP1roG wave function model.")

        .def("geminalCoefficients",
            &GQCP::QCModel::vAP1roG::geminalCoefficients,
            "Return the corresponding geminal coefficients of these AP1roG model parameters."
        )

        .def("lagrangeMultipliers",
            [ ] (const GQCP::QCModel::vAP1roG& qc_model) {
                return qc_model.lagrangeMultipliers().asMatrix();
            },
            "Return the Lagrange multipliers."
        )
    ;
}


}  // namespace gqcpy
