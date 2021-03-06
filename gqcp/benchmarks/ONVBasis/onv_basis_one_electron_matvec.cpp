/**
 *  A benchmark executable that tests the one-electron matrix-vector product for a full spin-unresolved ONV basis. The number of spinors is 28, the number of electrons varies from 5 to 9.
 */

#include <benchmark/benchmark.h>

#include "ONVBasis/SpinUnresolvedONVBasis.hpp"
#include "Operator/SecondQuantized/SQOneElectronOperator.hpp"


static void CustomArguments(benchmark::internal::Benchmark* b) {
    for (int i = 5; i < 10; ++i) {  // need int instead of size_t
        b->Args({28, i});  // spinors, electrons
    }
}


static void matvec(benchmark::State& state) {

    // Prepare a full spin-unresolved ONV basis
    const size_t M = state.range(0);  // number of spinors
    const size_t N = state.range(1);  // number of electrons
    GQCP::SpinUnresolvedONVBasis onv_basis (M, N);

    // Create a random one-electron operator
    const auto one_op_par = GQCP::QCMatrix<double>::Random(M, M);
    GQCP::ScalarSQOneElectronOperator<double> sq_one_op {one_op_par};

    const auto diagonal = onv_basis.evaluateOperatorDiagonal(sq_one_op);
    const auto x = onv_basis.randomExpansion();

    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        GQCP::VectorX<double> matvec = onv_basis.evaluateOperatorMatrixVectorProduct(sq_one_op, x, diagonal);

        benchmark::DoNotOptimize(matvec);  // make sure that the variable is not optimized away by compiler
    }

    state.counters["Spinors"] = M;
    state.counters["Electrons"] = N;
    state.counters["Dimension"] = onv_basis.get_dimension();
}


BENCHMARK(matvec)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);
BENCHMARK_MAIN();
