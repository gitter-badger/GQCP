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
#include "Operator/SecondQuantized/ModelHamiltonian/HoppingMatrix.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;


namespace gqcpy {


void bindHoppingMatrix(py::module& module) {
    py::class_<GQCP::HoppingMatrix<double>>(module, "HoppingMatrix", "The Hubbard hopping matrix.")

        /**
         *  The C++ constructor that is bound here takes a 'SquareMatrix' argument, but we can't implicitly convert a numpy array to our own Eigen-derived type. Therefore, we choose a Python static method for binding the C++ constructor.
         */
        .def_static("FromAdjacencyMatrix",
            [ ] (const Eigen::MatrixXd& A, const double t, const double U) {

                return GQCP::HoppingMatrix<double>{GQCP::SquareMatrix<double>{A}, t, U};
            },
            py::arg("A"),
            py::arg("t"),
            py::arg("U"),
            "Return the Hubbard hopping matrix from an adjacency matrix and Hubbard model parameters U and t."
        )


        .def_static("FromCSLine",
            [ ] (const std::string& cs_line) {
                return GQCP::HoppingMatrix<double>::FromCSLine(cs_line);
            },
            "Return the hopping matrix that corresponds to the given comma-separated line."
        )
    ;
}


}  // namespace gqcpy
