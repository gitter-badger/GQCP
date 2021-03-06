/**
 *  A benchmark executable for DOCI calculations on CO in a 6-31G basisset. This system as (K = 28) number of spatial orbitals and (N = 14) electrons and a total seniority-zero dimension of 1184040.
 */

#include <benchmark/benchmark.h>

#include "Mathematical/Optimization/Eigenproblem/Davidson/DavidsonSolver.hpp"
#include "ONVBasis/SeniorityZeroONVBasis.hpp"
#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "QCMethod/CI/CI.hpp"
#include "QCMethod/CI/CIEnvironment.hpp"


static void test_case(benchmark::State& state) {

    // Read in the molecular Hamiltonian.
    const auto sq_hamiltonian = GQCP::SQHamiltonian<double>::ReadFCIDUMP("data/co_631g_klaas.FCIDUMP");
    const auto K = sq_hamiltonian.dimension();
    const auto N_P = 7;  // number of electron pairs


    // Set up a seniority-zero (doubly-occupied) ONV basis.
    const GQCP::SeniorityZeroONVBasis onv_basis (K, N_P);


    // Specify an initial guess for the Davidson solver.
    const auto initial_guess = onv_basis.hartreeFockExpansion();
    auto environment = GQCP::CIEnvironment::Iterative(sq_hamiltonian, onv_basis, initial_guess);
    auto solver = GQCP::EigenproblemSolver::Davidson();


    // Code inside this loop is measured repeatedly.
    for (auto _ : state) {
        const auto electronic_energy = GQCP::QCMethod::CI<GQCP::SeniorityZeroONVBasis>(onv_basis).optimize(solver, environment).groundStateEnergy();

        benchmark::DoNotOptimize(electronic_energy);  // make sure that the variable is not optimized away by compiler
    }

    state.counters["Spatial orbitals"] = K;
    state.counters["Electron pairs"] = N_P;
    state.counters["Dimension"] = onv_basis.dimension();
}


BENCHMARK(test_case)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();
