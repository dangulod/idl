#ifndef LOGNORMAL_H__
#define LOGNORMAL_H__

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include <idl/distributions/distribution.hpp>

namespace idl
{
    namespace distributions
    {
        class LogNormal: public Distribution
        {
        private:
            double m_mean, m_sd;
        public:
            LogNormal() = delete;
            LogNormal(double mean, double sd);
            ~LogNormal() = default;

            bool operator ==(const LogNormal &rhs) const;
            bool operator !=(const LogNormal &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mean() const;
            double get_sd() const;

            pt::ptree to_ptree();
            static LogNormal from_ptree(const pt::ptree & value);
        };
    }
}

#endif