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
#include "QCMethod/HF/RHFSCFSolver.hpp"

#include <pybind11/pybind11.h>


namespace py = pybind11;


namespace gqcpy {


void bindRHFSCFSolver(py::module& module) {
    py::class_<GQCP::RHFSCFSolver<double>>(module, "RHFSCFSolver", "A restricted Hartree-Fock self-consistent field solver factory.")

        .def_static("Plain",
            [ ] (const double threshold = 1.0e-08, const size_t maximum_number_of_iterations = 128) {
                return GQCP::RHFSCFSolver<double>::Plain(threshold, maximum_number_of_iterations);
            },
            py::arg("threshold") = 1.0e-08,
            py::arg("maximum_number_of_iterations") = 128,
            "Return a plain RHF SCF solver that uses the norm of the difference of two consecutive density matrices as a convergence criterion."
        )

        .def_static("DensityDamped", 
            [ ] (const double alpha, const double threshold = 1.0e-08, const size_t maximum_number_of_iterations = 128) {
                return GQCP::RHFSCFSolver<double>::DensityDamped(alpha, threshold, maximum_number_of_iterations);
            },
            py::arg("alpha"),
            py::arg("threshold") = 1.0e-08,
            py::arg("maximum_number_of_iterations") = 128,
            "Return a density-damped RHF SCF solver that uses the norm of the difference of two consecutive density matrices as a convergence criterion."
        )

        .def_static("DIIS",
            [ ] (const double threshold, const size_t maximum_number_of_iterations) {
                return GQCP::RHFSCFSolver<double>::DIIS(threshold, maximum_number_of_iterations);
            },
            py::arg("threshold") = 1.0e-08,
            py::arg("maximum_number_of_iterations") = 128
        )
    ;
}


}  // namespace gqcpy