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
            return object.get_recovery(value);
        })
        .def("get_default_probability", &idl::IDLParams::get_default_probability)
        .def("__repr__", [](const idl::IDLParams & object)
        {
            return "IDLParams class";
        })
        .def("add", [](idl::IDLParams & object,
                       std::string name, 
                       idl::Recovery recovery)
        {
            object.add(name, recovery);
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
            std::ostringstream out;

            out << "RatingPD class with "         <<
                    std::to_string(object.size()) <<
                    " ratings";

            return out.str();
        })
    ;
    py::class_<idl::Recovery, std::shared_ptr<idl::Recovery>>(m, "Recovery")
        .def(py::init<double>())
        .def(py::init<std::shared_ptr<idl::distributions::Distribution>>())
        .def("generate_recovery", py::overload_cast<>(&idl::Recovery::generate_recovery))
        .def("generate_recovery", py::overload_cast<size_t, bool>(&idl::Recovery::generate_recovery),
            py::arg("seed"), py::arg("fixed") = false)
        .def_property_readonly("distribution", &idl::Recovery::get_distribution)
        .def("is_stochastic", &idl::Recovery::is_stochastic)
        .def("__repr__", [](idl::Recovery & object)
        {
            std::ostringstream out;

            if (!object.is_stochastic())
            {
                out << "Fixed Recovery: " << object.generate_recovery();
            } else
            {
                auto obj = py::cast(object.get_distribution());
                out << "Stochastic Recovery: " << obj.attr("__repr__")();
            }
            
            return out.str();
        })
    ;
}
