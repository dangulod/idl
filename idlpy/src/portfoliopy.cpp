#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_portfolio(py::module_ &m) {
    py::class_<idl::Portfolio>(m, "Portfolio")
        .def(py::init<idl::Factor&, idl::IDLParams&>())
        .def_property_readonly("factor", &idl::Portfolio::get_factor)
        .def_property_readonly("IDLParams", &idl::Portfolio::get_IDLParams)
        .def_property_readonly("number_of_factors", &idl::Portfolio::get_number_of_factors)
        .def("size", &idl::Portfolio::size)
        .def("__len__", &idl::Portfolio::size)
        .def("to_json", &idl::Portfolio::to_json)
        .def_static("from_json", &idl::Portfolio::from_json)
        .def("__getitem__", [] (idl::Portfolio & object, 
                                std::string id)
        {
            return idl::Position(*object[id].get());
        })
        .def("__add__", [](idl::Portfolio & object, 
                           std::string id,
                           idl::Position & value)
        {
            object.add_position(id, value);
        })
        .def("__repr__", [](const idl::Portfolio & object)
        {
            std::string str("Portfolio class with ");
            str.append(std::to_string(object.size()));
            str.append(" counterparties");

            return str;
        })
        .def("get_correlation_structure", &idl::Portfolio::correlation_sructure)
        .def("get_scenarios", &idl::Portfolio::get_scenarios, 
            py::arg("n"), py::arg("seed"), py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("get_CWIs", &idl::Portfolio::get_CWIs, 
            py::arg("n"), py::arg("seed"), py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("marginal_loss", py::overload_cast<size_t, size_t, size_t>(&idl::Portfolio::marginal_loss), 
            py::arg("n"), py::arg("seed"), py::arg("n_threads") = std::thread::hardware_concurrency())
        ;
    py::class_<idl::Position>(m, "Position")
        .def(py::init<const double &, const double &, const unsigned &, const unsigned &, const unsigned &, size_t &>())
        .def(py::init<const double &, const double &, idl::WeightsDimension &, size_t &>())
        .def("__eq__", &idl::Position::operator==)
        .def_property_readonly("jtd", &idl::Position::get_jtd)
        .def_property_readonly("notional", &idl::Position::get_notional)
        .def_property_readonly("weight_dimension", &idl::Position::get_weight_dimension)
        .def_property_readonly("rating", &idl::Position::get_rating)
        .def_property_readonly("region", &idl::Position::get_region)
        .def_property_readonly("sector", &idl::Position::get_sector)
        .def_property_readonly("idio_seed", &idl::Position::get_idio_seed)
        .def("__repr__", [](const idl::Position & object)
        {
            std::string str("[");
            str.append(double_to_string(object.get_jtd()));
            str.append(", ");
            str.append(double_to_string(object.get_notional()));
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
