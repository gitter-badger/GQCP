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


#include "QCModel/CI/LinearExpansion.hpp"
#include "Operator/SecondQuantized/SQOneElectronOperator.hpp"
#include "Processing/RDM/OneRDM.hpp"


namespace GQCP {


/**
 *  Calculate the electric polarizability from the linear wave function response
 * 
 *  @param F_p          the electric response force (d^2E/dFdp)
 *  @param response     the linear wave function response
 * 
 *  @return the components of the electric polarizability
 */
Matrix<double, 3, 3> calculateElectricPolarizability(const Matrix<double, Dynamic, 3>& F_p, const Matrix<double, Dynamic, 3>& response);

/**
 *  Calculate the Dyson 'amplitudes' (the coefficients of a Dyson orbital) between two wave function expressed in the same spinor basis 
 * 
 *  @param linear_expansion1        a wave function in a spin-unresolved ONV basis  
 *  @param linear_expansion2        a wave function in a spin-unresolved ONV basis containing one fewer electron and the same amount of orbitals that is expressed in the same basis
 *
 * 
 *  @return a vector with the Dyson orbital amplitudes  
 */
VectorX<double> calculateDysonOrbitalCoefficients(const LinearExpansion<SpinResolvedONVBasis>& linear_expansion1, const LinearExpansion<SpinResolvedONVBasis>& linear_expansion2) {


    const auto onv_basis1 = linear_expansion1.onvBasis();
    const auto onv_basis2 = linear_expansion2.onvBasis();

    if ((onv_basis1.get_N_alpha() - onv_basis2.get_N_alpha() != 0) && (onv_basis1.get_N_beta() - onv_basis2.get_N_beta() != 1)) {
        if ((onv_basis1.get_N_alpha() - onv_basis2.get_N_alpha() != 1) && (onv_basis1.get_N_beta() - onv_basis2.get_N_beta() != 0)) {
            throw std::runtime_error("properties::calculateDysonOrbital(LinearExpansion, LinearExpansion): linear_expansion2 is not expressed in a spin-resolved ONV basis with one fewer electron than linear_expansion1");
        }
    } 

    // Initialize environment variables

    // The 'passive' ONV basis is the ONV basis that is equal for both wave functions
    // The 'target' ONV basis has an electron difference of one
    // We initialize the variables for the case in which they differ in one beta electron, if this isn't the case, we will update it later 
    auto passive_onv_basis1 = onv_basis1.get_fock_space_alpha();
    auto passive_onv_basis2 = onv_basis2.get_fock_space_alpha();
    auto target_onv_basis1 = onv_basis1.get_fock_space_beta();
    auto target_onv_basis2 = onv_basis2.get_fock_space_beta();

    // Mod variables relate to the modification of the address jump in coefficient index according to the ordering of the spin ONVs 
    size_t passive_mod1 = target_onv_basis1.get_dimension();
    size_t passive_mod2 = target_onv_basis2.get_dimension();
    size_t target_mod = 1;

    // If instead the ONV bases differ by one alpha electron we re-assign the variables to match the algorithm
    if ((onv_basis1.get_N_alpha() - onv_basis2.get_N_alpha() == 1) && (onv_basis1.get_N_beta() - onv_basis2.get_N_beta() == 0)) {
        passive_onv_basis1 = target_onv_basis1;
        passive_onv_basis2 = target_onv_basis2;
        target_onv_basis1 = onv_basis1.get_fock_space_alpha();
        target_onv_basis2 = onv_basis2.get_fock_space_alpha();
        
        passive_mod1 = 1;
        passive_mod2 = 1;
        target_mod = passive_onv_basis1.get_dimension();
    }

    const auto& ci_coeffs1 = linear_expansion1.coefficients();
    const auto& ci_coeffs2 = linear_expansion2.coefficients();

    VectorX<double> dyson_coeffs = VectorX<double>::Zero(onv_basis1.get_K());

    // The actual algorithm to determine the Dyson amplitudes
    // Since we want to calculate the overlap between two wave functions, the ONVs should have an equal number of electrons
    // We therefore iterate over the ONVs of the 'target' ONV basis, which all have an electron more, and annihilate in one of the orbitals (let the index of that orbital be called 'p')
    // By calculating the overlap in the (N-1)-ONV basis, we can calculate the contributions to the  'p'-th coefficient (i.e. the Dyson amplitude) of the Dyson orbital
    SpinUnresolvedONV onv = target_onv_basis1.makeONV(0);

    for (size_t It = 0; It < target_onv_basis1.get_dimension(); It++) {  // It loops over addresses of the target ONV basis
        int sign = -1;  // total phase factor of all the annihilations that have occurred
        for (size_t e = 0; e < target_onv_basis1.get_N(); e++) {  // loop over electrons in the ONV
        
            // Annihilate on the corresponding orbital, to make sure we can calculate overlaps in the (N-1)-'target' ONV basis
            sign *= -1; 
            size_t p = onv.get_occupation_index(e);
            onv.annihilate(p);

            // Now, we calculate the overlap in the (N-1)-'target' ONV basis
            // In order to access the correct coefficients for the, we need the address of the resulting (annihilated) ONV inside the 'target' ONV basis
            size_t address = target_onv_basis2.getAddress(onv.get_unsigned_representation());

            double coeff = 0;
            for (size_t Ip = 0; Ip < passive_onv_basis1.get_dimension(); Ip++) {  // Ip loops over the addresses of the passive ONV basis
                coeff += sign * ci_coeffs1(It * target_mod + Ip * passive_mod1) * ci_coeffs2(address * target_mod + Ip * passive_mod2);  // access the indices of the coefficient vectors
            }
            dyson_coeffs(p) += coeff;
            onv.create(p);  // allow the iteration to continue with the original ONV
        }

        if (It < target_onv_basis1.get_dimension() - 1) {  // prevent last permutation to occur
            target_onv_basis1.setNextONV(onv);
        }
    }  // target address (It) loop
    
    return dyson_coeffs;
}


}  // namespace GQCP
