#ifndef LOGNORMAL_H__
#define LOGNORMAL_H__

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

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_mean() const;
            double get_sd() const;
        };
    }
}

#endif