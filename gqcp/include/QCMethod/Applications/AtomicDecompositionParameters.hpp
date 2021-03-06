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


#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "Molecule/Molecule.hpp"


namespace GQCP {


/**
 *  A class that holds the collection of Hamiltonians that represent different molecular decompositions
 */
class AtomicDecompositionParameters {
private:
    SQHamiltonian<double> molecular_hamiltonian_parameters;  // the Hamiltonian of the complete molecule

    std::vector<SQHamiltonian<double>> net_atomic_parameters;  // vector of net atomic Hamiltonians
    std::vector<SQHamiltonian<double>> interaction_parameters;  // vector of interaction Hamiltonians
    std::vector<SQHamiltonian<double>> atomic_parameters;  // vector of the total atomic contributions


public:

    /*
     *  CONSTRUCTORS
     */
    AtomicDecompositionParameters() = default;
    

    /**
     *  @param molecular_hamiltonian_parameters     the complete molecular Hamiltonian
     *  @param net_atomic_parameters                collection of net atomic Hamiltonians
     *  @param interaction_parameters               collection of atomic interaction Hamiltonians
     *  @param atomic_parameters                    collection of atomic Hamiltonians
     */
    AtomicDecompositionParameters (const SQHamiltonian<double>& molecular_hamiltonian_parameters, const std::vector<SQHamiltonian<double>>& net_atomic_parameters, const std::vector<SQHamiltonian<double>>& interaction_parameters, const std::vector<SQHamiltonian<double>>& atomic_parameters);



    /*
     *  NAMED CONSTRUCTORS
     */

    /**
     *  Constructs net atomic, atomic and atomic interaction Hamiltonians in the AO basis for a diatomic molecule AB.
     *   the term "Nuclear" concerns how the electronic nuclear integrals (potential energy) are decomposed. The potential energy
     *   for basis functions on atom A for the charge on B are included in the interaction energy and not in the net atomic energy.
     *
     *  @param molecule     the molecule for which the AtomicDecompositionParameters should be calculated
     *  @param basisset     the name of the basisset corresponding to the AO basis
     *
     *  @return Atomic decomposed parameters:
     *      - net atomic parameters with:
     *          - one-electron nuclear integrals separated by atomic core and the atomic basis functions centered on that atom.
     *          - one-electron kinetic integrals separated per set of atomic basis functions centered on an atom.
     *          - two-electron integrals separated per set of atomic basis functions centered on an atom.
     *      - interaction parameters with:
     *          - remaining one- and two-electron contributions when deducting the net atomic parameters from the total Hamiltonian
     *          - scalar : nuclear repulsion
     *      - atomic parameters, HamiltonianParameters with:
     *          - net atomic parameters + interaction parameters/2
     *
     *  Ordering of the atomic Hamiltonians are dependant on the ordering of the atoms in the molecule
     *   for the molecule AB:
     *      net_atomic_parameters will contains parameters for A then B.
     *      interaction_parameters will contain parameters for the AB interaction.
     *      atomic_parameters will contain parameters for A then B.
     */
    static AtomicDecompositionParameters Nuclear(const Molecule& molecule, const std::string& basisset_name);



    /*
     *  GETTERS
     */
    const SQHamiltonian<double>& get_molecular_hamiltonian_parameters() const { return this->molecular_hamiltonian_parameters; }
    const std::vector<SQHamiltonian<double>>& get_net_atomic_parameters() const { return this->net_atomic_parameters; }
    const std::vector<SQHamiltonian<double>>& get_interaction_parameters() const { return this->interaction_parameters; }
    const std::vector<SQHamiltonian<double>>& get_atomic_parameters() const { return this->atomic_parameters; }
};


}  // namespace GQCP
