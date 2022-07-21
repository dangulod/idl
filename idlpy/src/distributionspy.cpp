#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value)
{
    return std::regex_replace(std::to_string(value), std::regex("0*$|\\.0*$"), "");
}

void ex_distributions(py::module_ &m) {
    py::class_<idl::distributions::Distribution, std::shared_ptr<idl::distributions::Distribution>>(m, "Distribution")
        .def("cdf", py::vectorize(&idl::distributions::Distribution::cdf),
             py::arg("x"))
        .def("pdf", py::vectorize(&idl::distributions::Distribution::pdf),
             py::arg("x"))
        .def("quantile", py::vectorize(&idl::distributions::Distribution::quantile),
             py::arg("p"))
        .def("mean", &idl::distributions::Distribution::mean)
        .def("__call__", [](idl::distributions::Distribution & object, size_t n, unsigned long seed)
        {
            arma::vec output = object(n, seed);

            auto result        = py::array_t<double>(n);
            auto result_buffer = result.request();
            double *result_ptr = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, output.memptr(), output.size() * sizeof(double));

            return result;
        }, py::arg("n"), py::arg("seed"))
    ;
    py::class_<idl::distributions::LogNormal, 
               idl::distributions::Distribution,
               std::shared_ptr<idl::distributions::LogNormal>>(m, "LogNormal")
        .def(py::init<double, double>(), py::arg("mu") = 0, py::arg("sd") = 1)
        .def_property_readonly("mu", &idl::distributions::LogNormal::get_mu)
        .def_property_readonly("sd", &idl::distributions::LogNormal::get_sd)
        .def("__repr__", [](const idl::distributions::LogNormal & object)
        {
            std::ostringstream out;

            out << "LogNormal("                       <<
                    double_to_string(object.get_mu()) <<
                    ", "                              <<
                    double_to_string(object.get_sd()) <<
                    ")";

            return out.str();
        })
        ;
    py::class_<idl::distributions::Uniform, idl::distributions::Distribution, std::shared_ptr<idl::distributions::Uniform>>(m, "Uniform")
        .def(py::init<double, double>(), py::arg("lower") = 0, py::arg("upper") = 1)
        .def_property_readonly("lower", &idl::distributions::Uniform::get_lower)
        .def_property_readonly("upper", &idl::distributions::Uniform::get_upper)
        .def("__repr__", [](const idl::distributions::Uniform & object)
        {
            std::ostringstream out;

            out << "Uniform("                            <<
                    double_to_string(object.get_lower()) <<
                    ", "                                 <<
                    double_to_string(object.get_upper()) <<
                    ")";

            return out.str();
        })
        ;
    py::class_<idl::distributions::Normal, idl::distributions::Distribution, std::shared_ptr<idl::distributions::Normal>>(m, "Normal")
        .def(py::init<double, double>(), py::arg("mu") = 0, py::arg("sd") = 1)
        .def_property_readonly("mu", &idl::distributions::Normal::get_mu)
        .def_property_readonly("sd", &idl::distributions::Normal::get_sd)
        .def("__repr__", [](const idl::distributions::Normal & object)
        {
            std::ostringstream out;
            
            out << "Normal("                          <<
                    double_to_string(object.get_mu()) <<
                    ", "                              <<
                    double_to_string(object.get_sd()) <<
                    ")";

            return out.str();
        })
        ;
    py::class_<idl::distributions::Beta, idl::distributions::Distribution, std::shared_ptr<idl::distributions::Beta>>(m, "Beta")
        .def(py::init<double, double, double, double>(), py::arg("shape1") = 0.5,
        py::arg("shape2") = 0.5, py::arg("a") = 0, py::arg("b") = 1)
        .def_property_readonly("shape1", &idl::distributions::Beta::get_shape1)
        .def_property_readonly("shape2", &idl::distributions::Beta::get_shape2)
        .def_property_readonly("a", &idl::distributions::Beta::get_a)
        .def_property_readonly("b", &idl::distributions::Beta::get_b)
        .def("__repr__", [](const idl::distributions::Beta & object)
        {
            std::ostringstream out;

            out << "Beta("                                <<
                    double_to_string(object.get_shape1()) <<
                    ", "                                  <<
                    double_to_string(object.get_shape1()) << 
                    ", "                                  <<
                    double_to_string(object.get_a())      <<
                    ", "                                  <<
                    double_to_string(object.get_b())      <<
                    ")";

            return out.str();
        })
        ;
}
