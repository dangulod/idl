#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_portfolio(py::module_ &m) {
    py::class_<idl::Portfolio>(m, "Portfolio")
        .def(py::init<idl::Factor& >())
        .def("size", &idl::Portfolio::size)
        .def_property_readonly("number_of_factors", &idl::Portfolio::get_number_of_factors)
        .def("__len__", &idl::Portfolio::size)
        .def("to_json", &idl::Portfolio::to_json)
        .def_static("from_json", &idl::Portfolio::from_json)
        .def_property_readonly("factor", &idl::Portfolio::get_factor)
        .def("__getitem__", &idl::Portfolio::operator[])
        .def("__add__", [](idl::Portfolio & object, 
                           idl::Counterparty & value)
        {
            object + value;
        })
        .def("__repr__", [](const idl::Portfolio & object)
        {
            std::string str("Portfolio class with ");
            str.append(std::to_string(object.size()));
            str.append(" counterparties");

            return str;
        })
        .def("get_scenarios", &idl::Portfolio::get_scenarios, 
            py::arg("n"), py::arg("seed"), py::arg("n_threads") = std::thread::hardware_concurrency())
        ;
    py::class_<idl::Counterparty>(m, "Counterparty")
        .def(py::init<const double &, const unsigned &, const unsigned &, const unsigned &>())
        .def(py::init<const double & , idl::WeightsDimension &>())
        .def("__eq__", &idl::Counterparty::operator==)
        .def_property_readonly("jtd", &idl::Counterparty::get_jtd)
        .def_property_readonly("weight_dimension", &idl::Counterparty::get_weight_dimension)
        .def_property_readonly("rating", &idl::Counterparty::get_rating)
        .def_property_readonly("region", &idl::Counterparty::get_region)
        .def_property_readonly("sector", &idl::Counterparty::get_sector)
        .def("__repr__", [](const idl::Counterparty & object)
        {
            std::string str("[");
            str.append(double_to_string(object.get_jtd()));
            str.append(", ");
            str.append(std::to_string(object.get_rating()));
            str.append(", ");
            str.append(std::to_string(object.get_region()));
            str.append(", ");
            str.append(std::to_string(object.get_sector()));
            str.append("]");

            return str;
        })
        ;
}
