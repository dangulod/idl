#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_arma(py::module_ &m) {
    py::class_<arma::Mat<double>>(m, "arma_mat")
        .def_property_readonly("n_rows", [](const arma::Mat<double> & object)
        {
            return object.n_rows;
        })
        .def_property_readonly("n_cols",[](const arma::Mat<double> & object)
        {
            return object.n_cols;
        })
        .def_property_readonly("__len__",[](const arma::Mat<double> & object)
        {
            return object.size();
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
            std::string str("Arma matrix (");
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
    py::class_<arma::vec>(m, "arma_vec")
        .def_property_readonly("n_rows", [](const arma::vec & object)
        {
            return object.n_rows;
        })
        .def_property_readonly("n_cols",[](const arma::vec & object)
        {
            return object.n_cols;
        })
        .def_property_readonly("__len__",[](const arma::vec & object)
        {
            return object.size();
        })
        .def("__getitem__", [](const arma::vec & object, size_t ii)
        {

            if (ii >= object.size())
            {
                throw std::out_of_range("Arma Vec indexes out of range");
            }

            return object.at(ii);
        })
        .def("__repr__", [](const arma::vec & object)
        {
            std::string str("Arma vec (");
            str.append(std::to_string(object.size()));
            str.append(")");

            return str;
        })
        .def("to_numpy", [](const arma::vec & object)
        {
            auto result        = py::array_t<double>(object.size());
            auto result_buffer = result.request();
            double *result_ptr    = (double *) result_buffer.ptr;

            // copy std::vector -> py::array 
            std::memcpy(result_ptr, object.memptr(), object.size() * sizeof(double));

            return result;
        })
        ;
}
