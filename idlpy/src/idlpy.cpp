#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <regex>
#include <idl.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

std::string double_to_string(double value);

void ex_distributions(py::module_ &);
void ex_factor(py::module_ &);
void ex_portfolio(py::module_ &);
void ex_arma(py::module_ &);
void ex_risk_params(py::module_ &);

PYBIND11_MODULE(_idlpy, m) {
    ex_distributions(m);
    ex_factor(m);
    ex_portfolio(m);
    ex_arma(m);
    ex_risk_params(m);
    
    m.attr("__version__") = MACRO_STRINGIFY(IDLPY_VERSION);
}
