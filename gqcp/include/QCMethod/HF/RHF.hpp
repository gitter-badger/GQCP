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


#include "Mathematical/Algorithm/IterativeAlgorithm.hpp"
#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "QCMethod/QCObjective.hpp"
#include "QCMethod/QCStructure.hpp"
#include "QCMethod/HF/RHFSCFEnvironment.hpp"
#include "QCModel/HF/RHF.hpp"

#include <type_traits>


namespace GQCP {
namespace QCMethod {


/**
 *  The restricted Hartree-Fock quantum chemical method
 * 
 *  @tparam _Scalar             the type of scalar that is used for the expansion of the spatial orbitals in their underlying scalar basis
 */
template <typename _Scalar>
class RHF {

public:
    using Scalar = _Scalar;


public:

    /*
     *  PUBLIC METHODS
     */

    /**
     *  Optimize the RHF wave function model: find the parameters satisfy the given objective.
     * 
     *  @tparam QCObjective         the type of the objective
     *  @tparam Solver              the type of the solver
     * 
     *  @param objective            the objective that should be fulfilled in order to consider the model's parameters as 'optimal'
     *  @param solver               the solver that will try to optimize the parameters
     *  @param environment          the environment, which acts as a sort of calculation space for the solver
     */
    template <typename QCObjective, typename Solver>
    QCStructure<QCModel::RHF<Scalar>> optimize(const QCObjective& objective, Solver& solver, RHFSCFEnvironment<Scalar>& environment) const {

        // The RHF method's responsibility is to try to optimize the parameters of its method, given a solver and associated environment.
        solver.perform(environment);

        // To make a QCStructure<QCModel::RHF<Scalar>>, we need the electronic energy, coefficient matrix, orbital energies and the number of electrons.
        // Furthermore, the current RHF SCF solvers only find the ground state wave function parameters, so the QCStructure only needs to contain the parameters for one state.
        const auto& E_electronic = environment.electronic_energies.back();
        const auto& C = environment.coefficient_matrices.back();
        const auto& orbital_energies = environment.orbital_energies.back();
        const auto& N_P = environment.N / 2;

        const QCModel::RHF<Scalar> rhf_parameters (N_P, orbital_energies, C);

        // Now that we have constructed an instance of the QCModel, we should check if the objective is fulfilled
        if (!objective.isSatisfiedWith(rhf_parameters)) {
            throw std::runtime_error("QCModel::RHF::optimize(const QCObjective&, Solver&, RHFSCFEnvironment<Scalar>&): The solver produced a solution that does not fulfill the objective.");
        }
        return QCStructure<QCModel::RHF<Scalar>>({E_electronic}, {rhf_parameters});
    }
};


}  // namespace QCMethod
}  // namespace GQCP
