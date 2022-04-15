cmake_minimum_required(VERSION 2.8)

file(
    GLOB 
    TEST_SOURCES 
    *_tests.cpp
)

add_executable(
    IDL_tests
    ${TEST_SOURCES}
)

target_link_libraries(
    IDL_tests
    PRIVATE
    GTest::GTest
    IDL::Core
    arma::arma
)

add_test(NAME IDL_tests COMMAND IDL_tests)
set_property(TARGET IDL_tests PROPERTY FOLDER "tests")

if(BUILD_IDL_COVERAGE)
    SETUP_TARGET_FOR_COVERAGE(
        coverage
        IDL_tests
        coverage
    )
endif()
