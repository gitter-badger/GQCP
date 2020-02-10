/**
 *  A benchmark executable for DOCI calculations on CO in a 6-31G basisset.
 */

#include <benchmark/benchmark.h>

#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "QCMethod/CI/CISolver.hpp"
#include "QCMethod/CI/HamiltonianBuilder/DOCI.hpp"


static void test_case(benchmark::State& state) {

    const auto sq_hamiltonian = GQCP::SQHamiltonian<double>::ReadFCIDUMP("data/co_631g_klaas.FCIDUMP");

    // The system contains 14 electrons and requires 28 basis functions. For DOCI, we can use a SpinUnresolvedONV basis for 14 spatial orbitals and 7 electrons to represent this situation.  TODO: this should change: SpinUnresolvedONVBasis shouldn't be used for DOCI.
    const auto K = sq_hamiltonian.dimension();
    const auto N_P = 7;
    const GQCP::SpinUnresolvedONVBasis onv_basis (K, N_P);  // dim = 1184040


    // Solve the DOCI eigenvalue problem with a Davidson solver
    const GQCP::DOCI doci (onv_basis);
    const GQCP::VectorX<double> initial_guess = onv_basis.HartreeFockExpansion();
    GQCP::DavidsonSolverOptions solver_options (initial_guess);

    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        GQCP::CISolver ci_solver (doci, sq_hamiltonian);
        ci_solver.solve(solver_options);

        benchmark::DoNotOptimize(ci_solver);  // make sure the variable is not optimized away by compiler
    }

    state.counters["Spatial orbitals"] = K;
    state.counters["Electron pairs"] = N_P;
    state.counters["Dimension"] = onv_basis.get_dimension();  // dim = 1184040
}


BENCHMARK(test_case)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();
