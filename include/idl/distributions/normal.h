#ifndef DIST_NORM_H__
#define DIST_NORM_H__

#include <armadillo>
#include <idl/utils/stats.h>
#include <idl/distributions/distribution.hpp>
#include <boost/math/special_functions/erf.hpp>

namespace idl
{
    namespace distributions
    {
        class Normal: public Distribution
        {
        private:
            double m_mean, m_sd;
        public:
            Normal() = delete;
            Normal(double mean, double sd);
            ~Normal() = default;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mean() const;
            double get_sd() const;
        };
    } // namespace distributions
} // namespace idl

#endif