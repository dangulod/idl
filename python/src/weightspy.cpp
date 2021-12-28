#include <pybind11/pybind11.h>
#include <idl.h>

namespace py = pybind11;

PYBIND11_MODULE(weights, m) {
    py::class_<idl::WeightsDimension>(m, "WeightsDimension")
        .def(py::init<const unsigned &, const unsigned &, const unsigned &>())
        .def("get_rating", &idl::WeightsDimension::get_rating)
        .def("get_region", &idl::WeightsDimension::get_region)
        .def("get_sector", &idl::WeightsDimension::get_sector)
        .def("to_string", &idl::WeightsDimension::to_string)
        .def_static("from_string", &idl::WeightsDimension::from_string);
}
