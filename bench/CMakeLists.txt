cmake_minimum_required(VERSION 2.8)

macro(add_IDL_bench _BENCH_NAME)
    add_executable(${_BENCH_NAME}_bench ${_BENCH_NAME}_bench.cpp)
    target_link_libraries(${_BENCH_NAME}_bench PRIVATE
        GBench::GBench
        IDL::Core
        )
    # Group under the "bench" project folder in IDEs such as Visual Studio.
    set_property(TARGET ${_BENCH_NAME}_bench PROPERTY FOLDER "bench")
endmacro()

add_IDL_bench(scenarios)
add_IDL_bench(simulation)