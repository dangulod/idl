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
        class Normal: public Distribution,
            std::enable_shared_from_this<Normal>
        {
        private:
            double m_mu, m_sd;
        public:
            Normal() = delete;
            Normal(double mu, double sd);
            ~Normal() = default;

            bool operator ==(const Normal &rhs) const;
            bool operator !=(const Normal &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mu() const;
            double get_sd() const;

            double mean();

            pt::ptree to_ptree();
            static Normal from_ptree(const pt::ptree & value);
        };
    } // namespace distributions
} // namespace idl

#endif