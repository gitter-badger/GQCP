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
#define BOOST_TEST_MODULE "ERNewtonLocalizer"

#include <boost/test/unit_test.hpp>

#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "Mathematical/Optimization/IterativeIdentitiesHessianModifier.hpp"
#include "OrbitalOptimization/Localization/ERNewtonLocalizer.hpp"


BOOST_AUTO_TEST_CASE ( localization_index_raises ) {

    // Check if the Edmiston-Ruedenberg localization index is raised after a localization procedure
    auto h2o = GQCP::Molecule::ReadXYZ("data/h2o.xyz");
    size_t N_P = h2o.numberOfElectrons()/2;

    GQCP::SingleParticleBasis<double, GQCP::GTOShell> sp_basis (h2o, "STO-3G");
    auto sq_hamiltonian = GQCP::SQHamiltonian<double>::Molecular(sp_basis, h2o);  // in an AO basis
    sq_hamiltonian.LowdinOrthonormalize();  // in the Löwdin basis


    double D_before = sq_hamiltonian.calculateEdmistonRuedenbergLocalizationIndex(N_P);

    auto hessian_modifier = std::make_shared<GQCP::IterativeIdentitiesHessianModifier>();
    GQCP::ERNewtonLocalizer localizer (N_P, hessian_modifier, 1.0e-04);
    localizer.optimize(sq_hamiltonian);  // if converged, the Hamiltonian is in the localized basis

    double D_after = sq_hamiltonian.calculateEdmistonRuedenbergLocalizationIndex(N_P);

    BOOST_CHECK(D_after > D_before);
}
