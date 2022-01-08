#include <benchmark/benchmark.h>
#include <idl.h>

static void SCENARIOS_BENCH(benchmark::State& state) {
    
    using namespace idl;

    idl::Factor idl_factor(0, 0, 0);

    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    
    idl::Portfolio portfolio(idl_factor);
  
  while (state.KeepRunning()) {
    portfolio.get_scenarios(1e5, 123456789);
  }
}

BENCHMARK(SCENARIOS_BENCH)
    ->Args({100})
    ->Args({1000})
    ->Args({10000});

BENCHMARK_MAIN();
