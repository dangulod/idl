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
    py::class_<idl::distributions::LogNormal>(m, "LogNormal")
        .def(py::init<double, double>())
        .def_property_readonly("mean", &idl::distributions::LogNormal::get_mean)
        .def_property_readonly("sd", &idl::distributions::LogNormal::get_sd)
        .def("__repr__", [](const idl::distributions::LogNormal & object)
        {
            std::string str("LogNormal(");
            str.append(double_to_string(object.get_mean()));
            str.append(", ");
            str.append(double_to_string(object.get_sd()));
            str.append(")");

            return str;
        })
        .def("cdf", py::vectorize(&idl::distributions::LogNormal::cdf))
        .def("pdf", py::vectorize(&idl::distributions::LogNormal::pdf))
        .def("quantile", py::vectorize(&idl::distributions::LogNormal::quantile))
        .def("__call__", [](idl::distributions::LogNormal & object, size_t n, unsigned long seed)
        {
            arma::vec output = object(n, seed);

            auto result        = py::array_t<double>(n);
            auto result_buffer = result.request();
            double *result_ptr = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, output.memptr(), output.size() * sizeof(double));

            return result;
        })
        ;
    py::class_<idl::distributions::Uniform>(m, "Uniform")
        .def(py::init<double, double>())
        .def_property_readonly("lower", &idl::distributions::Uniform::get_lower)
        .def_property_readonly("upper", &idl::distributions::Uniform::get_upper)
        .def("__repr__", [](const idl::distributions::Uniform & object)
        {
            std::string str("Uniform(");
            str.append(double_to_string(object.get_lower()));
            str.append(", ");
            str.append(double_to_string(object.get_upper()));
            str.append(")");

            return str;
        })
        .def("cdf", py::vectorize(&idl::distributions::Uniform::cdf))
        .def("pdf", py::vectorize(&idl::distributions::Uniform::pdf))
        .def("quantile", py::vectorize(&idl::distributions::Uniform::quantile))
        .def("__call__", [](idl::distributions::Uniform & object, size_t n, unsigned long seed)
        {
            arma::vec output = object(n, seed);

            auto result        = py::array_t<double>(n);
            auto result_buffer = result.request();
            double *result_ptr = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, output.memptr(), output.size() * sizeof(double));

            return result;
        })
        ;
    py::class_<idl::distributions::Normal>(m, "Normal")
        .def(py::init<double, double>())
        .def_property_readonly("mean", &idl::distributions::Normal::get_mean)
        .def_property_readonly("sd", &idl::distributions::Normal::get_sd)
        .def("__repr__", [](const idl::distributions::Normal & object)
        {
            std::string str("Normal(");
            str.append(double_to_string(object.get_mean()));
            str.append(", ");
            str.append(double_to_string(object.get_sd()));
            str.append(")");

            return str;
        })
        .def("cdf", py::vectorize(&idl::distributions::Normal::cdf))
        .def("pdf", py::vectorize(&idl::distributions::Normal::pdf))
        .def("quantile", py::vectorize(&idl::distributions::Normal::quantile))
        .def("__call__", [](idl::distributions::Normal & object, size_t n, unsigned long seed)
        {
            arma::vec output = object(n, seed);

            auto result        = py::array_t<double>(n);
            auto result_buffer = result.request();
            double *result_ptr = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, output.memptr(), output.size() * sizeof(double));

            return result;
        })
        ;
    py::class_<idl::distributions::Beta>(m, "Beta")
        .def(py::init<double, double, double, double>())
        .def_property_readonly("shape1", &idl::distributions::Beta::get_shape1)
        .def_property_readonly("shape2", &idl::distributions::Beta::get_shape2)
        .def_property_readonly("a", &idl::distributions::Beta::get_a)
        .def_property_readonly("b", &idl::distributions::Beta::get_b)
        .def("__repr__", [](const idl::distributions::Beta & object)
        {
            std::string str("Beta(");
            str.append(double_to_string(object.get_shape1()));
            str.append(", ");
            str.append(double_to_string(object.get_shape1()));
            str.append(", ");
            str.append(double_to_string(object.get_a()));
            str.append(", ");
            str.append(double_to_string(object.get_b()));
            str.append(")");

            return str;
        })
        .def("cdf", py::vectorize(&idl::distributions::Beta::cdf))
        .def("pdf", py::vectorize(&idl::distributions::Beta::pdf))
        .def("quantile", py::vectorize(&idl::distributions::Beta::quantile))
        .def("__call__", [](idl::distributions::Beta & object, size_t n, unsigned long seed)
        {
            arma::vec output = object(n, seed);

            auto result        = py::array_t<double>(n);
            auto result_buffer = result.request();
            double *result_ptr = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, output.memptr(), output.size() * sizeof(double));

            return result;
        })
        ;
}
