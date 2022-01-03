#ifndef DIST_NORM_H__
#define DIST_NORM_H__

#include <armadillo>
#include <idl/utils/stats.h>
#include <boost/math/special_functions/erf.hpp>

namespace idl
{
    namespace distributions
    {
        namespace normal
        {
            double quantile(double p, double mean = 0, double sd = 1);
            arma::vec quantile(const arma::vec p, double mean = 0, double sd = 1);
            double cdf(double x, double mean = 0, double sd = 1);
            arma::vec cdf(const arma::vec x, double mean = 0, double sd = 1);
            double pdf(double x, double mean = 0, double sd = 1);
            arma::vec pdf(const arma::vec x, double mean = 0, double sd = 1);
            double random_s(unsigned long seed, double mean = 0, double sd = 1);
            arma::vec random_v(size_t n, unsigned long seed, double mean = 0, double sd = 1);
        } // namespace normal
    } // namespace distributions
} // namespace idl

#endif