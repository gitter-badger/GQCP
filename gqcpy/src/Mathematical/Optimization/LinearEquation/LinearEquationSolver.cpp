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
#include "Mathematical/Optimization/LinearEquation/LinearEquationSolver.hpp"

#include <pybind11/pybind11.h>


namespace py = pybind11;


namespace gqcpy {


void bindLinearEquationSolver(py::module& module) {

    auto module_linear_equation_solver = module.def_submodule("LinearEquationSolver");

    module_linear_equation_solver.def("HouseholderQR",
        [ ] () {
            return GQCP::LinearEquationSolver<double>::HouseholderQR();
        },
        "Return a linear equations solver that uses the Householder QR algorithm."
    );

    module_linear_equation_solver.def("ColPivHouseholderQR",
        [ ] () {
            return GQCP::LinearEquationSolver<double>::ColPivHouseholderQR();
        },
        "Return a linear equations solver that uses the Householder QR (with column-pivoting) algorithm."
    );
}


}  // namespace gqcpy
