#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_risk_params(py::module_ &m) {
    py::class_<idl::IDLParams>(m, "IDLParams")
        .def(py::init<>())
        .def("get_recovery", [](idl::IDLParams & object, std::string value)
        {
            return idl::distributions::Beta(*object.get_recovery(value).get());
        })
        .def("get_default_probability", &idl::IDLParams::get_default_probability)
        .def("__repr__", [](const idl::IDLParams & object)
        {
            std::string str("IDLParams class");

            return str;
        })
        .def("add", [](idl::IDLParams & object,
                       std::string name, 
                       idl::distributions::Beta dist)
        {
            object.add(name, dist);
        })
        .def("add", [](idl::IDLParams & object,
                       std::string name, 
                       idl::RatingPD ratingPD)
        {
            object.add(name, ratingPD);
        })
    ;
    py::class_<idl::RatingPD>(m, "RatingPD")
        .def(py::init<>())
        .def("size", &idl::RatingPD::size)
        .def("__len__", &idl::RatingPD::size)
        .def("add", &idl::RatingPD::add)
        .def("__getitem__", [](idl::RatingPD & object,  unsigned int index)
        {
            return object[index];
        })
        .def("__call__", [](idl::RatingPD & object, unsigned int index)
        {
            return object[index];
        })
        .def("__eq__", &idl::RatingPD::operator==)
        .def("__repr__", [](const idl::RatingPD & object)
        {
            std::string str("RatingPD class with ");
            str.append(std::to_string(object.size()));
            str.append(" ratings");

            return str;
        })
    ;
}
