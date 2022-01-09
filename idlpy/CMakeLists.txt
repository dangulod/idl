cmake_minimum_required(VERSION 2.8)

project(idlpy)

add_subdirectory(pybind11)

file(GLOB PYMODEL_SOURCES
    src/*.cpp
    )

pybind11_add_module(_idlpy ${PYMODEL_SOURCES})

target_link_libraries(_idlpy PRIVATE IDL::Core)

target_compile_definitions(_idlpy
                           PRIVATE IDLPY_VERSION=${IDLPY_VERSION})
