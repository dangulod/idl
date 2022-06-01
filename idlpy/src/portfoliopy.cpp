#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_portfolio(py::module_ &m) {
    py::class_<idl::Portfolio>(m, "Portfolio")
        .def(py::init<idl::Factor&, idl::IDLParams&>(), py::arg("factor"), py::arg("idl_params"))
        .def_property_readonly("factor", &idl::Portfolio::get_factor)
        .def_property_readonly("IDLParams", &idl::Portfolio::get_IDLParams)
        .def_property_readonly("number_of_factors", &idl::Portfolio::get_number_of_factors)
        .def("size", &idl::Portfolio::size)
        .def("__len__", &idl::Portfolio::size)
        .def("to_json", &idl::Portfolio::to_json, py::arg("file"))
        .def_static("from_json", &idl::Portfolio::from_json, py::arg("file"))
        .def("__getitem__", [] (idl::Portfolio & object, 
                                std::string id)
        {
            return object[id];
        }, py::arg("id"))
        .def("add_position", &idl::Portfolio::add_position, py::arg("id"), py::arg("value"))
        .def("__repr__", [](const idl::Portfolio & object)
        {
            std::ostringstream out;

            out << "Portfolio class with "        <<
                    std::to_string(object.size()) <<
                    " counterparties";

            return out.str();
        })
        .def("keys", [](const idl::Portfolio & object)
        {
            return py::make_key_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        .def("__iter__", [](const idl::Portfolio & object)
        {
            return py::make_value_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        .def("items", [](const idl::Portfolio & object)
        {
            return py::make_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        .def("values", [](const idl::Portfolio & object)
        {
            return py::make_value_iterator(object.begin(), object.end()); 
        }, py::keep_alive<0, 1>())
        .def("get_correlation_structure", &idl::Portfolio::correlation_sructure)
        .def("get_scenarios", &idl::Portfolio::get_scenarios, 
            py::arg("n"), py::arg("n_replenishment"), py::arg("seed"),
            py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("get_CWIs", &idl::Portfolio::get_CWIs, 
            py::arg("n"), py::arg("n_replenishment"), py::arg("seed"), py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("component_loss", py::overload_cast<size_t, size_t, size_t, bool, bool, size_t>(&idl::Portfolio::component_loss), 
            py::arg("n"), py::arg("n_replenishment"), py::arg("seed"), py::arg("diversification") = 0, py::arg("hedge") = true,
            py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("component_loss", py::overload_cast<std::vector<size_t>, size_t, size_t, bool, bool, size_t>(&idl::Portfolio::component_loss),
            py::arg("scenarios_ids"), py::arg("n_replenishment"), py::arg("seed"), py::arg("diversification") = 0,
            py::arg("hedge") = true, py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("total_loss", py::overload_cast<size_t, size_t, size_t, bool, bool, size_t>(&idl::Portfolio::total_loss), 
            py::arg("n"), py::arg("n_replenishment"), py::arg("seed"), py::arg("diversification") = 0, py::arg("hedge") = true,
            py::arg("n_threads") = std::thread::hardware_concurrency())
        .def("total_loss", py::overload_cast<std::vector<size_t>, size_t, size_t, bool, bool, size_t>(&idl::Portfolio::total_loss),
            py::arg("scenarios_ids"), py::arg("n_replenishment"), py::arg("seed"), py::arg("diversification") = 0,
            py::arg("hedge") = true, py::arg("n_threads") = std::thread::hardware_concurrency())
        ;
}
