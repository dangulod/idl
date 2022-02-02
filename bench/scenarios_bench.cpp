#include <benchmark/benchmark.h>
#include <idl.h>

static void SCENARIOS_BENCH(benchmark::State& state) {
    
    using namespace idl;

    idl::Factor idl_factor(0, 0, 0, {0.1, 0.1, 0.1});

    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    
    idl::IDLParams idl_params;

    idl_params.add("all", idl::distributions::Beta(1, 1, 0, 1));
    idl_params.add("mortgages", idl::distributions::Beta(1, 1, 0, 1));
    
    idl::RatingPD sov;
    sov.add(1, 0.1);
    sov.add(2, 0.15);
    sov.add(3, 0.2);
    sov.add(4, 0.25);
    sov.add(5, 0.3);
    sov.add(6, 0.35);
    sov.add(7, 0.4);

    idl_params.add("SOV", sov);
    
    idl::Portfolio portfolio(idl_factor, idl_params);
  
  while (state.KeepRunning()) {
    portfolio.get_scenarios(1e5, 123456789);
  }
}

BENCHMARK(SCENARIOS_BENCH)
    ->Args({100})
    ->Args({1000})
    ->Args({10000});

BENCHMARK_MAIN();
