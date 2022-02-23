cmake_minimum_required(VERSION 3.5)

project(IDL LANGUAGES CXX)

macro(IDL_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

IDL_option(BUILD_IDL_TESTS "Set to ON to build IDL tests." ON)
IDL_option(USE_SYSTEM_GTEST
    "If ON, this project will look in the system paths for an installed gtest library" OFF)
IDL_option(BUILD_IDL_BENCHMARK "Set to ON to build IDL benchmark." ON)
IDL_option(USE_SYSTEM_GBENCH
    "If ON, this project will look in the system paths for an installed google benchmark library" OFF)
IDL_option(BUILD_IDLPY
    "If ON, this project will try to build the idlpy library" ON)
if(BUILD_IDL_TESTS)
IDL_option(BUILD_IDL_COVERAGE "Set to ON to build IDL coverage" ON)
endif()

if (BUILD_IDLPY AND NOT DEFINED IDLPY_VERSION)
    set(IDLPY_VERSION "dev")
endif()

if(BUILD_IDL_TESTS AND BUILD_IDL_COVERAGE)
  include( cmake_aux/codeCoverage.cmake )
  SET(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
endif()

file(GLOB MODEL_SOURCES
    src/*.cpp
    )

include(FetchContent)
FetchContent_Declare(armadillo-code
    GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code
    GIT_TAG 10.8.x)
FetchContent_MakeAvailable(armadillo-code)

add_library(armadillo_int INTERFACE)
target_link_libraries(armadillo_int INTERFACE armadillo)
target_include_directories(armadillo_int INTERFACE ${armdillo_SOURCE_DIR}/include)

add_library(arma::arma ALIAS armadillo_int)

add_library(IDL SHARED ${MODEL_SOURCES})
add_library(IDL::Core ALIAS IDL)

target_link_libraries(IDL arma::arma)
include_directories(${PROJECT_SOURCE_DIR}/include)

if(BUILD_IDL_TESTS)
    if(USE_SYSTEM_GTEST)
        find_package(GTest)
    endif()
    if(NOT USE_SYSTEM_GTEST OR NOT GTEST_FOUND)
        include(FetchContent)
        message(STATUS "Not using system gtest, using built-in googletest project instead.")
        SET(BUILD_GMOCK OFF CACHE BOOL "Use some expat option")
        SET(INSTALL_GTEST OFF CACHE BOOL "Use some expat option")

        if(MSVC)
            # By default, GTest compiles on Windows in CRT static linkage mode. We use this
            # variable to force it into using the CRT in dynamic linkage (DLL), just as CPR
            # does.
            set(gtest_force_shared_crt ON CACHE BOOL "Force gtest to use the shared c runtime")
        endif()
        FetchContent_Declare(googletest
                                    GIT_REPOSITORY https://github.com/google/googletest.git
                                    GIT_TAG release-1.10.0)
        FetchContent_MakeAvailable(googletest)

        add_library(gtest_int INTERFACE)
        target_link_libraries(gtest_int INTERFACE gtest)
        target_include_directories(gtest_int INTERFACE ${googletest_SOURCE_DIR}/include)

        add_library(GTest::GTest ALIAS gtest_int)

        # Group under the "tests/gtest" project folder in IDEs such as Visual Studio.
        set_property(TARGET gtest PROPERTY FOLDER "tests/gtest")
        set_property(TARGET gtest_main PROPERTY FOLDER "tests/gtest")
    endif()
endif()

if(BUILD_IDL_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(BUILD_IDL_BENCHMARK)
    if(USE_SYSTEM_GBENCH)
        find_package(benchmark)
    endif()
    if(NOT USE_SYSTEM_GTEST OR NOT GTEST_FOUND)
        include(FetchContent)
        message(STATUS "Not using system google benchmark, using built-in google benchmark project instead.")
        set(BENCHMARK_ENABLE_INSTALL OFF)
        set(BENCHMARK_ENABLE_TESTING OFF)
        set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
        set(BENCHMARK_USE_BUNDLED_GTEST OFF)        

        FetchContent_Declare(googlebenchmark
                                    GIT_REPOSITORY https://github.com/google/benchmark.git
                                    GIT_TAG v1.6.0)
        FetchContent_MakeAvailable(googlebenchmark)

        add_library(gbenchmark_int INTERFACE)
        target_link_libraries(gbenchmark_int INTERFACE benchmark)
        target_include_directories(gbenchmark_int INTERFACE ${benchmark_SOURCE_DIR}/include)

        add_library(GBench::GBench ALIAS gbenchmark_int)

        # Group under the "tests/gtest" project folder in IDEs such as Visual Studio.
        set_property(TARGET benchmark PROPERTY FOLDER "bench/benchmark")
        set_property(TARGET benchmark_main PROPERTY FOLDER "bench/benchmark")
    endif()
endif()

if(BUILD_IDL_BENCHMARK)
    add_subdirectory(bench)
endif()

if (BUILD_IDLPY)
    add_subdirectory(idlpy)
endif()
