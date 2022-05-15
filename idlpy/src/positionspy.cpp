#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_position(py::module_ &m) {
    py::class_<idl::Hedge, std::shared_ptr<idl::Hedge>>(m, "Hedge")
        .def(py::init<const double &, const double &, const unsigned &>())
        .def("__eq__", &idl::Hedge::operator==)
        .def_property_readonly("jtd", &idl::Hedge::get_jtd)
        .def_property_readonly("notional", &idl::Hedge::get_notional)
        .def_property_readonly("rating", &idl::Hedge::get_rating)
        .def("__repr__", [](const idl::Hedge & object)
        {
            std::ostringstream out;

            auto obj = py::cast(object);

            out << py::str(obj.attr("__class__").attr("__name__")) <<
                   ": ["                                           <<
                   double_to_string(object.get_jtd())              <<
                   ", "                                            <<
                   double_to_string(object.get_notional())         <<
                   ", "                                            <<
                   std::to_string(object.get_rating())             <<
                   "]";

            return out.str();
        })
        ;
    py::class_<idl::Position, std::shared_ptr<idl::Position>>(m, "Position")
        .def(py::init<const double &, const double &, const unsigned &, const unsigned &, const unsigned &, size_t &,
             std::vector<std::shared_ptr<idl::Hedge>>>(),
             py::arg("jtd"), py::arg("notional"), py::arg("rating"), py::arg("region"),
             py::arg("sector"), py::arg("idio_seed"), py::arg("hedges") =  std::vector<std::shared_ptr<idl::Hedge>>())
        .def(py::init<const double &, const double &, idl::WeightsDimension &, size_t &,
             std::vector<std::shared_ptr<idl::Hedge>>>(),
             py::arg("jtd"), py::arg("notional"), py::arg("w_dim"), py::arg("idio_seed"),
             py::arg("hedges") =  std::vector<std::shared_ptr<idl::Hedge>>())
        .def("__eq__", &idl::Position::operator==)
        .def("__add__", &idl::Position::operator+)
        .def("_add_hedge", &idl::Position::operator+)
        .def("__iadd__", &idl::Position::operator+=)
        .def_property_readonly("hedges", &idl::Position::get_hedges)
        .def_property_readonly("jtd", [](const idl::Position & object)
        {
            return object.get_jtd(true);
        })
        .def_property_readonly("jtd_unhedged", [](const idl::Position & object)
        {
            return object.get_jtd(false);
        })
        .def_property_readonly("notional", [](const idl::Position & object)
        {
            return object.get_notional(true);
        })
        .def_property_readonly("notional_unhedged", [](const idl::Position & object)
        {
            return object.get_notional(false);
        })
        .def_property_readonly("weight_dimension", &idl::Position::get_weight_dimension)
        .def_property_readonly("rating", &idl::Position::get_rating)
        .def_property_readonly("region", &idl::Position::get_region)
        .def_property_readonly("sector", &idl::Position::get_sector)
        .def_property_readonly("idio_seed", &idl::Position::get_idio_seed)
        .def("__repr__", [](const idl::Position & object)
        {
            std::ostringstream out;

            auto obj = py::cast(object);

            out << py::str(obj.attr("__class__").attr("__name__")) <<
                   ": ["                                           <<
                   double_to_string(object.get_jtd())              <<
                   ", "                                            <<
                   double_to_string(object.get_notional())         <<
                   ", "                                            <<
                   std::to_string(object.get_rating())             <<
                   ", "                                            <<
                   std::to_string(object.get_region())             <<
                   ", "                                            <<
                   std::to_string(object.get_sector())             <<
                   "]";

            return out.str();
        })
        ;
}