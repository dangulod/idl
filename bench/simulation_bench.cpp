#include <benchmark/benchmark.h>
#include <idl.h>

static void FACTOR_BENCH(benchmark::State& state) {
    
    using namespace idl;

    idl::Factor idl_factor(0, 0, 0);

    idl_factor.add({1, 0, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 0, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 0, 1}, {0.1, 0.1, 0.1});
  
  while (state.KeepRunning()) {
    idl_factor.get_default();
  }
}

BENCHMARK(FACTOR_BENCH)
    ->Args({100})
    ->Args({1000})
    ->Args({10000})
    ->Args({100000});

BENCHMARK_MAIN();
