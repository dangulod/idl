#include <benchmark/benchmark.h>
#include <idl.h>

static void FACTOR_BENCH(benchmark::State& state) {
    
    using namespace idl;

    idl::Factor idl_factor(0, 0, 0, {0.1, 0.1, 0.1});

    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    
    idl::IDLParams idl_params;

    idl_params.add("all",
                   idl::Recovery(std::make_shared<idl::distributions::Beta>(idl::distributions::Beta(1, 1, 0, 1))));
    idl_params.add("mortgages",
                   idl::Recovery(std::make_shared<idl::distributions::Beta>(idl::distributions::Beta(1, 1, 0, 1))));

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

    int ii(0);

    while(ii++ < 100)
    {
        std::shared_ptr<idl::Position> count(std::make_shared<idl::Position>(idl::Position(900, 1000, 5, 1, 3, ii)));

        portfolio.add_position(std::to_string(ii), count);
    }
  
  while (state.KeepRunning()) {
    portfolio.component_loss(1e3, 123456789);
  }
}

BENCHMARK(FACTOR_BENCH)
    ->Args({100})
    ->Args({1000})
    ->Args({10000})
    ->Args({100000})
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
