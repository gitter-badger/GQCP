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
#ifndef GQCP_LIBINTCOMMUNICATOR_HPP
#define GQCP_LIBINTCOMMUNICATOR_HPP


#include "Basis/AOBasis.hpp"
#include "Molecule.hpp"
#include "Operator/OneElectronOperator.hpp"
#include "Operator/TwoElectronOperator.hpp"

#include <boost/preprocessor.hpp>  // include preprocessor before libint to fix libint-boost bug
#include <libint2.hpp>


namespace GQCP {


/**
 *  A singleton class that takes care of interfacing with the Libint2 (version 2.3.1) C++ API
 *
 *  Singleton class template from: https://stackoverflow.com/a/1008289
 */
class LibintCommunicator {
private:
    // PRIVATE METHODS - SINGLETON
    /**
     *  Private constructor as required by the singleton class design
     */
    LibintCommunicator();

    /**
     *  Private destructor as required by the singleton class design
     */
    ~LibintCommunicator();


    // PRIVATE STRUCTS
    typedef struct {} empty;  // empty_pod is a private typedef for libint2::Engine, so we copy it over


public:
    // PUBLIC METHODS - SINGLETON
    /**
     *  @return the static singleton instance
     */
    static LibintCommunicator& get();

    /**
     *  Remove the public copy constructor and the public assignment operator
     */
    LibintCommunicator(LibintCommunicator const& libint_communicator) = delete;
    void operator=(LibintCommunicator const& libint_communicator) = delete;


    // PUBLIC METHODS - INTERFACING
    /**
     *  @param atoms        the GQCP-atoms that should be interfaced
     *
     *  @return libint2-atoms, interfaced from the given atoms
     */
    std::vector<libint2::Atom> interface(const std::vector<Atom>& atoms) const;

    /**
     *  @param shellset     the GQCP ShellSet that should be interfaced
     *
     *  @return a libint2::BasisSet, interfaced from the GQCP ShellSet
     */
    libint2::BasisSet interface(const ShellSet& shellset) const;

    /**
     *  @param shell        the GQCP shell that should be interfaced
     *
     *  @return a libint2::Shell, interfaced from the GQCP Shell
     */
    libint2::Shell interface(const Shell& shell) const;


    // PUBLIC METHODS - INTEGRALS
    /**
     *  @tparam N               the number of operator components
     *
     *  @param operator_type    the name of the operator as specified by the enumeration
     *  @param basisset         the libint2 basis set representing the AO basis
     *  @param parameters       the parameters for the integral engine, as specified by libint2
     *
     *  @return an array of N OneElectronOperators corresponding to the matrix representations of the N components of the given operator type
     */
    template <size_t N>
    std::array<OneElectronOperator<double>, N> calculateOneElectronIntegrals(libint2::Operator operator_type, const libint2::BasisSet& basisset, const libint2::any& parameters = empty()) const {

        // Initialize the N components of the matrix representations of the operator
        const auto nbf = static_cast<size_t>(basisset.nbf());  // number of basis functions
        std::array<OneElectronOperator<double>, N> operator_components;
        for (auto& op : operator_components) {
            op = OneElectronOperator<double>::Zero(nbf, nbf);
        }


        // Construct the libint2 engine
        libint2::Engine engine (operator_type, basisset.max_nprim(), static_cast<int>(basisset.max_l()));
        engine.set_params(parameters);

        const auto shell2bf = basisset.shell2bf();  // create a map between (shell index) -> (basis function index)

        const auto& calculated_integrals = engine.results();  // vector that holds pointers to computed shell sets
        assert(calculated_integrals.size() == N);             // its size is N, so it holds N pointers to the first computed integral of an integral set


        // One-electron integrals are between two basis functions, so we'll need two loops
        // Libint calculates integrals between libint2::Shells, so we will loop over the shells in the basisset
        const auto nsh = static_cast<size_t>(basisset.size());  // number of shells
        for (auto sh1 = 0; sh1 != nsh; ++sh1) {  // shell 1
            for (auto sh2 = 0; sh2 != nsh; ++sh2) {  // shell 2
                // Calculate integrals between the two shells
                engine.compute(basisset[sh1], basisset[sh2]);  // this updates the pointers in calculated_integrals


                // Place the calculated integrals into the matrix representation(s): the integrals are stored in row-major form
                auto bf1 = shell2bf[sh1];  // (index of) first bf in sh1
                auto bf2 = shell2bf[sh2];  // (index of) first bf in sh2

                auto nbf_sh1 = basisset[sh1].size();  // number of basis functions in first shell
                auto nbf_sh2 = basisset[sh2].size();  // number of basis functions in second shell

                for (auto f1 = 0; f1 != nbf_sh1; ++f1) {  // f1: index of basis function within shell 1
                    for (auto f2 = 0; f2 != nbf_sh2; ++f2) { // f2: index of basis function within shell 2

                        for (size_t i = 0; i < N; i++) {
                            double computed_integral = calculated_integrals[i][f2 + f1 * nbf_sh2];  // integrals are packed in row-major form
                            operator_components[i](bf1 + f1, bf2 + f2) = computed_integral;
                        }

                    }
                }  // data access loops

            }
        }  // shell loops

        return operator_components;
    }


    /**
     *  @param operator_type    the name of the operator as specified by the enumeration
     *  @param ao_basis         the AO basis in which the two-electron operator should be expressed
     *
     *  @return the matrix representation of a two-electron operator in the given AO basis
     */
    TwoElectronOperator<double> calculateTwoElectronIntegrals(libint2::Operator operator_type, const AOBasis& ao_basis) const;




    /**
     *  @param ao_basis     the AO basis used for the calculation of the overlap integrals
     *
     *  @return the overlap integrals expressed in the given AO basis
     */
    OneElectronOperator<double> calculateOverlapIntegrals(const AOBasis& ao_basis) const;

    /**
     *  @param ao_basis     the AO basis used for the calculation of the kinetic integrals
     *
     *  @return the kinetic integrals expressed in the given AO basis
     */
    OneElectronOperator<double> calculateKineticIntegrals(const AOBasis& ao_basis) const;

    /**
     *  @param ao_basis     the AO basis used for the calculation of the nuclear attraction integrals
     *
     *  @return the nuclear attraction integrals expressed in the given AO basis
     */
    OneElectronOperator<double> calculateNuclearIntegrals(const AOBasis& ao_basis) const;

    /**
     *  @param ao_basis     the AO basis used for the calculation of the dipole repulsion integrals
     *  @param origin       the origin of the dipole
     *
     *  @return the Cartesian components of the electrical dipole operator, expressed in the given AO basis
     */
    std::array<OneElectronOperator<double>, 3> calculateDipoleIntegrals(const AOBasis& ao_basis, const Vector<double, 3>& origin=Vector<double, 3>::Zero()) const;

    /**
     *  @param ao_basis     the AO basis used for the calculation of the Coulomb repulsion integrals
     *
     *  @return the Coulomb repulsion integrals expressed in the given AO basis
     */
    TwoElectronOperator<double> calculateCoulombRepulsionIntegrals(const AOBasis& ao_basis) const;


};


}  // namespace GQCP


#endif  // GQCP_LIBINTCOMMUNICATOR_HPP
