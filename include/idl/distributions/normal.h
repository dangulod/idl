#ifndef DIST_NORM_H__
#define DIST_NORM_H__

#include <armadillo>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/math/special_functions/erf.hpp>

#include <idl/utils/stats.h>
#include <idl/distributions/distribution.hpp>

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

            bool operator ==(const Normal &rhs) const;
            bool operator !=(const Normal &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mean() const;
            double get_sd() const;

            pt::ptree to_ptree();
            static Normal from_ptree(const pt::ptree & value);
        };
    } // namespace distributions
} // namespace idl

#endif