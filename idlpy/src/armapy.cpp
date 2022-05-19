#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_arma(py::module_ &m) {
    m.def("test_arma_cube", [](const size_t n_rows, const size_t n_cols, const size_t n_slices)
    {
        return arma::cube(n_rows, n_cols, n_slices, arma::fill::randu);
    });
    m.def("test_arma_matrix", [](const size_t n_rows, const size_t n_cols)
    {
        return arma::mat(n_rows, n_cols, arma::fill::randu);
    });
    m.def("test_arma_vec", [](const size_t n)
    {
        return arma::vec(n, arma::fill::randu);
    });
    py::class_<arma::Cube<double>>(m, "arma_cube")
        .def_property_readonly("n_rows", [](const arma::Cube<double> & object)
        {
            return object.n_rows;
        })
        .def_property_readonly("n_cols",[](const arma::Cube<double> & object)
        {
            return object.n_cols;
        })
        .def_property_readonly("n_slices",[](const arma::Cube<double> & object)
        {
            return object.n_slices;
        })
        .def("__len__",[](const arma::Cube<double> & object)
        {
            return object.size();
        })
        .def("__getitem__", [](const arma::Cube<double> & object, size_t ii)
        {
            if (ii >= object.n_slices)
            {
                throw std::out_of_range("Arma Cube index out of range");
            }

            return object.slice(ii);
        })
        .def("__getitem__", [](const arma::Cube<double> & object, py::tuple ii)
        {
            if (ii.size() != 3)
            {
                throw std::invalid_argument("Arma Cube subscript operator must be called with 3 arguments");
            }

            if (py::cast<size_t>(ii[0]) >= object.n_rows | 
                py::cast<size_t>(ii[1]) >= object.n_cols | 
                py::cast<size_t>(ii[2]) >= object.n_slices)
            {
                throw std::out_of_range("Arma Cube indexes out of range");
            }

            return object.at(py::cast<size_t>(ii[0]),
                             py::cast<size_t>(ii[1]),
                             py::cast<size_t>(ii[2]));
        })
        .def("slice", [](const arma::Cube<double> & object, size_t ii)
        {
            if (ii >= object.n_slices)
            {
                throw std::out_of_range("Arma Cube index out of range");
            }

            return object.slice(ii);
        })
        .def("row", [](const arma::Cube<double> & object, size_t ii)
        {
            if (ii >= object.n_rows)
            {
                throw std::out_of_range("Arma Cube index out of range");
            }

            return arma::mat(arma::mat(object.row(ii)).t());
        })
        .def("col", [](const arma::Cube<double> & object, size_t ii)
        {
            if (ii >= object.n_cols)
            {
                throw std::out_of_range("Arma Cube index out of range");
            }

            return arma::mat(arma::mat(object.col(ii)).t());
        })
        .def("__repr__", [](const arma::Cube<double> & object)
        {
            std::ostringstream out;

            out << "Arma cube (" << 
                    std::to_string(object.n_rows)   <<
                    ", "                            << 
                    std::to_string(object.n_cols)   <<
                    ", "                            << 
                    std::to_string(object.n_slices) <<
                    ")";

            return out.str();
        })
        .def("print", [](const arma::Cube<double> & object)
        {
            object.print();
        })
        .def("to_numpy", [](const arma::Cube<double> & object)
        {
            ssize_t              ndim    = 3;
            std::vector<ssize_t> shape   = { static_cast<long>(object.n_slices),
                                             static_cast<long>(object.n_rows),
                                             static_cast<long>(object.n_cols) };
            std::vector<ssize_t> strides = { static_cast<ssize_t>(sizeof(double) * object.n_rows * object.n_cols),
                                             static_cast<ssize_t>(sizeof(double) * object.n_cols), 
                                             sizeof(double) };

            std::vector<double> output(object.size());

            auto it_output = output.begin();

            for (size_t it_slice = 0; it_slice < object.n_slices; it_slice++)
            {
                auto it_element = object.slice(it_slice).begin_row(0);

                while (it_element != object.slice(it_slice).end_row(object.n_rows - 1))
                {
                    (*it_output) = (*it_element);
                    ++it_element;
                    ++it_output;
                }
            }

            auto it_y = output.begin();

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

    py::class_<arma::Mat<double>>(m, "arma_mat")
        .def_property_readonly("n_rows", [](const arma::Mat<double> & object)
        {
            return object.n_rows;
        })
        .def_property_readonly("n_cols",[](const arma::Mat<double> & object)
        {
            return object.n_cols;
        })
        .def("__len__",[](const arma::Mat<double> & object)
        {
            return object.size();
        })
        .def("__getitem__", [](const arma::Mat<double> & object, size_t ii)
        {
            if (ii >= object.n_rows)
            {
                throw std::out_of_range("Arma Mat index out of range");
            }

            return static_cast<arma::vec>(object.row(ii).t());
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
        .def("row", [](const arma::Mat<double> & object, size_t ii)
        {
            if (ii >= object.n_rows)
            {
                throw std::out_of_range("Arma Mat index out of range");
            }

            return arma::vec(object.row(ii).t());
        })
        .def("col", [](const arma::Mat<double> & object, size_t ii)
        {
            if (ii >= object.n_cols)
            {
                throw std::out_of_range("Arma Mat index out of range");
            }

            return arma::vec(object.col(ii));
        })
        .def("__repr__", [](const arma::Mat<double> & object)
        {
            std::ostringstream out;

            out << "Arma matrix (" << 
                    std::to_string(object.n_rows) <<
                    ", "                          << 
                    std::to_string(object.n_cols) <<
                    ")";

            return out.str();
        })
        .def("print", [](const arma::Mat<double> & object)
        {
            object.print();
        })
        .def("to_numpy", [](const arma::Mat<double> & object)
        {
            ssize_t              ndim    = 2;
            std::vector<ssize_t> shape   = { static_cast<long>(object.n_rows), static_cast<long>(object.n_cols) };
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
        .def("t", [](const arma::Mat<double> & object)
        {
            return arma::mat(object.t());
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
        .def("__len__", [](const arma::vec & object)
        {
            return object.size();
        })
        .def("__iter__", [](const arma::vec & object)
        {
            return py::make_iterator(object.begin(), object.end());
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
            std::ostringstream out;

            out << "Arma vec (" <<
                    std::to_string(object.size()) << 
                    ")";
            
            return out.str();
        })
        .def("print", [](const arma::vec & object)
        {
            object.print();
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
