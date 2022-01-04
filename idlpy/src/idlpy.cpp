#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <idl.h>

namespace py = pybind11;

PYBIND11_MODULE(_idlpy, m) {
    m.def("qbeta", 
          py::vectorize(py::overload_cast<double, double, double, double, double>(&idl::distributions::beta::quantile)),
          py::arg("p"), py::arg("shape1"), py::arg("shape2"), py::arg("a") = 0, py::arg("b") = 1);
    m.def("pbeta", py::vectorize(py::overload_cast<double, double, double, double, double>(&idl::distributions::beta::cdf)),
          py::arg("x"), py::arg("shape1"), py::arg("shape2"), py::arg("a") = 0, py::arg("b") = 1);
    m.def("dbeta", py::vectorize(py::overload_cast<double, double, double, double, double>(&idl::distributions::beta::pdf)),
          py::arg("x"), py::arg("shape1"), py::arg("shape2"), py::arg("a") = 0, py::arg("b") = 1);
    m.def("qnorm", py::vectorize(py::overload_cast<double, double, double>(&idl::distributions::normal::quantile)),
          py::arg("p"), py::arg("mean") = 0, py::arg("sd") = 1);
    m.def("pnorm", py::vectorize(py::overload_cast<double, double, double>(&idl::distributions::normal::cdf)),
          py::arg("x"), py::arg("mean") = 0, py::arg("sd") = 1);
    m.def("dnorm", py::vectorize(py::overload_cast<double, double, double>(&idl::distributions::normal::pdf)),
          py::arg("x"), py::arg("mean") = 0, py::arg("sd") = 1);
    py::class_<arma::Mat<double>>(m, "arma_mat")
        .def_property_readonly("n_rows", [](const arma::Mat<double> & object)
        {
            return object.n_rows;
        })
        .def_property_readonly("n_cols",[](const arma::Mat<double> & object)
        {
            return object.n_cols;
        })
        .def("__getitem__", [](const arma::Mat<double> & object, py::tuple ii)
        {
            if (ii.size() != 2)
            {
                throw std::invalid_argument("Arma Mat subscript operator must be called with 2 arguments");
            }

            if (py::cast<size_t>(ii[0]) >= object.n_rows | py::cast<size_t>(ii[1]) >= object.n_cols)
            {
                throw std::out_of_range("Arma Mat indexes out of range");
            }

            return object.at(py::cast<size_t>(ii[0]), py::cast<size_t>(ii[1]));
        })
        .def("__repr__", [](const arma::Mat<double> & object)
        {
            std::string str("Arma matrix with (");
            str.append(std::to_string(object.n_rows));
            str.append(", ");
            str.append(std::to_string(object.n_cols));
            str.append(")");

            return str;
        })
        .def("to_numpy", [](const arma::Mat<double> & object)
        {
            ssize_t              ndim    = 2;
            std::vector<ssize_t> shape   = { static_cast<long>(object.n_rows) , static_cast<long>(object.n_cols) };
            std::vector<ssize_t> strides = { static_cast<ssize_t>(sizeof(double) * object.n_cols), sizeof(double) };

            std::vector<double> output(object.begin_row(0), object.end_row(object.n_rows - 1));

            return py::array(py::buffer_info(
                output.data(),                           /* data as contiguous array  */
                sizeof(double),                          /* size of one scalar        */
                py::format_descriptor<double>::format(), /* data type                 */
                ndim,                                    /* number of dimensions      */
                shape,                                   /* shape of the matrix       */
                strides                                  /* strides for each axis     */
            ));
        })
        ;
    py::class_<idl::Portfolio>(m, "Portfolio")
        .def(py::init<idl::Factor& >())
        .def("size", &idl::Portfolio::size)
        .def_property_readonly("number_of_factors", &idl::Portfolio::get_number_of_factors)
        .def("__len__", &idl::Portfolio::size)
        .def("to_json", &idl::Portfolio::to_json)
        .def_static("from_json", &idl::Portfolio::from_json)
        .def_property_readonly("factor", &idl::Portfolio::get_factor)
        .def("__add__", &idl::Portfolio::operator+)
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
            str.append(std::to_string(object.get_jtd()));
            str.append(",");
            str.append(std::to_string(object.get_rating()));
            str.append(",");
            str.append(std::to_string(object.get_region()));
            str.append(",");
            str.append(std::to_string(object.get_sector()));
            str.append("]");

            return str;
        })
        ;
    py::class_<idl::Factor>(m, "Factor")
        .def(py::init<const unsigned &, const unsigned &, const unsigned &>())
        .def(py::init<idl::WeightsDimension &>())
        .def_property_readonly("number_of_factors", &idl::Factor::get_number_of_factors)
        .def_property_readonly("default", &idl::Factor::get_default)
        .def("add", &idl::Factor::add)
        .def("to_json", &idl::Factor::to_json)
        .def_static("from_json", &idl::Factor::from_json)
        .def("size", &idl::Factor::size)
        .def("__len__", &idl::Factor::size)
        .def("__getitem__", &idl::Factor::operator[])
        .def("__getitem__", [](idl::Factor & object, py::array_t<unsigned int, py::array::c_style | py::array::forcecast> input)
        {
            auto input_buffer = input.request();
            unsigned int * uint__ptr = static_cast<unsigned int*>(input_buffer.ptr);

            if (input_buffer.size != 3)
            {
                throw std::invalid_argument("Invalid index size");
            }

            idl::WeightsDimension ii(uint__ptr[0], 
                                     uint__ptr[1], 
                                     uint__ptr[2]);

            return object[ii];
        })
        .def("__eq__", &idl::Factor::operator==)
        .def("__repr__", [](const idl::Factor & object)
        {
            std::string str("Factor class with ");
            str.append(std::to_string(object.size()));
            str.append(" weights");

            return str;
        })
        ;
    py::class_<idl::WeightsDimension>(m, "WeightsDimension")
        .def(py::init<const unsigned &, const unsigned &, const unsigned &>())
        .def_property_readonly("rating", &idl::WeightsDimension::get_rating)
        .def_property_readonly("region", &idl::WeightsDimension::get_region)
        .def_property_readonly("sector", &idl::WeightsDimension::get_sector)
        .def("to_string", &idl::WeightsDimension::to_string)
        .def("__eq__", &idl::WeightsDimension::operator==)
        .def("__ne__", &idl::WeightsDimension::operator!=)
        .def("__lt__", &idl::WeightsDimension::operator<)
        .def("__le__", &idl::WeightsDimension::operator<=)
        .def("__gt__", &idl::WeightsDimension::operator>)
        .def("__ge__", &idl::WeightsDimension::operator>=)
        .def_static("from_string", &idl::WeightsDimension::from_string)
        .def("__repr__", [](const idl::WeightsDimension & object)
        {
            std::string str("[");
            str.append(std::to_string(object.get_rating()));
            str.append(",");
            str.append(std::to_string(object.get_region()));
            str.append(",");
            str.append(std::to_string(object.get_sector()));
            str.append("]");

            return str;
        })
        ;
    py::class_<idl::Weights>(m, "Weights")
        .def_property_readonly("idiosyncratic", &idl::Weights::get_idiosyncratic)
        .def_property_readonly("R2", &idl::Weights::get_R2)
        .def(py::init([](py::array_t<double, py::array::c_style | py::array::forcecast> input) 
        {
            std::vector<double> array_vec(input.size());

            // copy py::array -> std::vector
            std::memcpy(array_vec.data(), input.data(), input.size() * sizeof(double));

            return idl::Weights(array_vec);
        }))
        .def("__getitem__", [](const idl::Weights & object, size_t ii)
        {
            if (ii >= object.size())
            {
                throw std::out_of_range("Weight index out of range");
            }

            return object.at(ii);
        })
        .def("__repr__", [](const idl::Weights & object)
        {
            std::string str("[");

            auto ii = object.begin();
            while (ii != object.end())
            {
                str.append(std::to_string(*ii));
                ii++;
                if (ii != object.end()) str.append(", ");
            }

            str.append("]");

            return str;
        })
        .def("to_numpy", [](const idl::Weights & object)
        {
            auto result        = py::array_t<double>(object.size());
            auto result_buffer = result.request();
            double *result_ptr    = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, object.memptr(), object.size() * sizeof(double));

            return result;
        })
        .def("size", [](const idl::Weights & object)
        {
            return object.size();
        })
        .def("__len__", [](const idl::Weights & object)
        {
            return object.size();
        })
        ;
}
