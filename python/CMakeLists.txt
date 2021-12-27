cmake_minimum_required(VERSION 2.8)

project(idlpy)

add_subdirectory(pybind11)

pybind11_add_module(weights src/weightspy.cpp)

target_link_libraries(weights IDL::Core)

target_compile_definitions(weights
                           PRIVATE VERSION_INFO=${IDL_VERSION})