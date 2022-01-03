#ifndef DIST_BETA_H__
#define DIST_BETA_H__

#include <armadillo>
#include <idl/utils/stats.h>
#include <boost/math/special_functions/beta.hpp>

namespace idl
{
    namespace distributions
    {
        namespace beta
        {
            double quantile(double p, double shape1, double shape2, double a = 0, double b = 1);
            arma::vec quantile(const arma::vec p, double shape1, double shape2, double a = 0, double b = 1);
            double cdf(double x, double shape1, double shape2, double a = 0, double b = 1);
            arma::vec cdf(const arma::vec x, double shape1, double shape2, double a = 0, double b = 1);
            double pdf(double x, double shape1, double shape2, double a = 0, double b = 1);
            arma::vec pdf(const arma::vec x, double shape1, double shape2, double a = 0, double b = 1);
            double random_s(unsigned long seed, double shape1, double shape2, double a = 0, double b = 1);
            arma::vec random_v(size_t n, unsigned long seed, double shape1, double shape2, double a = 0, double b = 1);
        } // namespace beta
    } // namespace distributions
} // namespace idl

#endif