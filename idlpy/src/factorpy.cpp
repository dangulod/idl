#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

namespace py = pybind11;

std::string double_to_string(double value);

void ex_factor(py::module_ &m) {
    py::class_<idl::Factor>(m, "Factor")
        .def(py::init<const unsigned &, const unsigned &, const unsigned &, idl::Weights>())
        .def(py::init<idl::WeightsDimension &, idl::Weights>())
        .def_property_readonly("number_of_factors", &idl::Factor::get_number_of_factors)
        .def_property_readonly("default", &idl::Factor::get_default)
        .def("add", &idl::Factor::add, py::arg("weightsDimension"), py::arg("weights"))
        .def("add", [](idl::Factor & object, 
                       py::array_t<unsigned int, py::array::c_style | py::array::forcecast> weightsDimension,
                       py::array_t<double, py::array::c_style | py::array::forcecast> weights)
        {
            auto weightsDimension_buffer = weightsDimension.request();
            unsigned int * uint__ptr = static_cast<unsigned int*>(weightsDimension_buffer.ptr);

            if (weightsDimension_buffer.size != 3)
            {
                throw std::invalid_argument("Invalid index size");
            }

            std::vector<double> weights_vec(weights.size());

            // copy py::array -> std::vector
            std::memcpy(weights_vec.data(), weights.data(), weights.size() * sizeof(double));

            object.add(idl::WeightsDimension(uint__ptr[0], uint__ptr[1], uint__ptr[2]), 
                       idl::Weights(weights_vec));
        }, py::arg("weightsDimension"), py::arg("weights"))
        .def("to_json", &idl::Factor::to_json, py::arg("file"))
        .def_static("from_json", &idl::Factor::from_json, py::arg("file"))
        .def("size", &idl::Factor::size)
        .def("__len__", &idl::Factor::size)
        .def("__getitem__", [](idl::Factor & object, idl::WeightsDimension & index)
        {
            return object[index];
        }, py::arg("index"))
        .def("__getitem__", [](idl::Factor & object, py::tuple index)
        {
            if (index.size() != 3)
            {
                throw std::invalid_argument("(Factor class) Index size must be 3");
            }

            idl::WeightsDimension ii(py::cast<unsigned int>(index[0]), 
                                     py::cast<unsigned int>(index[1]), 
                                     py::cast<unsigned int>(index[2]));

            return object[ii];
        }, py::arg("index"))
        .def("__call__", [](idl::Factor & object, const unsigned int rating, 
                            const unsigned int region, const unsigned int sector)
        {
            idl::WeightsDimension index(rating, 
                                        region, 
                                        sector);

            return object.at(index);
        }, py::arg("rating"), py::arg("region"), py::arg("sector"))
        .def("__call__", [](idl::Factor & object, idl::WeightsDimension & index)
        {
            return object.at(index);
        }, py::arg("index"))
        .def("__eq__", &idl::Factor::operator==, py::arg("rhs"))
        .def("__repr__", [](const idl::Factor & object)
        {
            std::ostringstream out;

            out << "Factor class with "           <<
                    std::to_string(object.size()) <<
                    " weights";

            return out.str();
        })
        .def("keys", [](const idl::Factor & object)
        {
            return py::make_key_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        .def("__iter__", [](const idl::Factor & object)
        {
            return py::make_value_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        .def("items", [](const idl::Factor & object)
        {
            return py::make_iterator(object.begin(), object.end());
        }, py::keep_alive<0, 1>())
        ;
    py::class_<idl::WeightsDimension>(m, "WeightsDimension")
        .def(py::init<const unsigned &, const unsigned &, const unsigned &>())
        .def(py::init([](py::array_t<unsigned int, py::array::c_style | py::array::forcecast> input) 
        {
            auto input_buffer = input.request();

            unsigned int * uint__ptr = static_cast<unsigned int*>(input_buffer.ptr);
            if (input.size() != 3)
            {
                throw std::invalid_argument("Invalid index size");
            }

            return idl::WeightsDimension(uint__ptr[0],
                                         uint__ptr[1],
                                         uint__ptr[2]);
        }), py::arg("input"))
        .def_property_readonly("rating", &idl::WeightsDimension::get_rating)
        .def_property_readonly("region", &idl::WeightsDimension::get_region)
        .def_property_readonly("sector", &idl::WeightsDimension::get_sector)
        .def("to_string", &idl::WeightsDimension::to_string)
        .def("__eq__", &idl::WeightsDimension::operator==, py::arg("rhs"))
        .def("__ne__", &idl::WeightsDimension::operator!=, py::arg("rhs"))
        .def("__lt__", &idl::WeightsDimension::operator< , py::arg("rhs"))
        .def("__le__", &idl::WeightsDimension::operator<=, py::arg("rhs"))
        .def("__gt__", &idl::WeightsDimension::operator> , py::arg("rhs"))
        .def("__ge__", &idl::WeightsDimension::operator>=, py::arg("rhs"))
        .def_static("from_string", &idl::WeightsDimension::from_string, py::arg("value"))
        .def("__repr__", [](const idl::WeightsDimension & object)
        {
            std::ostringstream out;

            out << "[" <<
                    std::to_string(object.get_rating()) <<
                    ", "                                <<
                    std::to_string(object.get_region()) <<
                    ", "                                <<
                    std::to_string(object.get_sector()) <<
                    "]";

            return out.str();
        })
        ;
    py::class_<idl::Weights, std::shared_ptr<idl::Weights>>(m, "Weights")
        .def(py::init([](py::array_t<double, py::array::c_style | py::array::forcecast> input) 
        {
            std::vector<double> array_vec(input.size());

            // copy py::array -> std::vector
            std::memcpy(array_vec.data(), input.data(), input.size() * sizeof(double));

            return idl::Weights(array_vec);
        }), py::arg("input"))
        .def_property_readonly("idiosyncratic", &idl::Weights::get_idiosyncratic)
        .def_property_readonly("R2", &idl::Weights::get_R2)
        .def("__eq__", &idl::Weights::operator==, py::arg("rhs"))
        .def("__getitem__", [](const idl::Weights & object, size_t index)
        {
            if (index >= object.size())
            {
                throw std::out_of_range("Weight index out of range");
            }

            return object.at(index);
        }, py::arg("index"))
        .def("__repr__", [](const idl::Weights & object)
        {
            std::ostringstream out;

            out << "[";

            auto index = object.begin();
            while (index != object.end())
            {
                out << double_to_string(*index);
                index++;
                if (index != object.end()) out << ", ";
            }

            out << "]";

            return out.str();
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
