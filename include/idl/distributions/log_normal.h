#ifndef LOGNORMAL_H__
#define LOGNORMAL_H__

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include <idl/distributions/distribution.hpp>

namespace idl
{
    namespace distributions
    {
        class LogNormal: public Distribution, std::enable_shared_from_this<LogNormal>
        {
        private:
            double m_mu, m_sd;
        public:
            LogNormal() = delete;
            LogNormal(double mu, double sd);
            ~LogNormal() = default;

            bool operator ==(const LogNormal &rhs) const;
            bool operator !=(const LogNormal &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mu() const;
            double get_sd() const;

            double mean();

            pt::ptree to_ptree();
            static LogNormal from_ptree(const pt::ptree & value);
        };
    }
}

#endif