#ifndef UNIFORM_H__
#define UNIFORM_H__

#include <idl/distributions/distribution.hpp>

namespace idl
{
    namespace distributions
    {
        class Uniform: public Distribution
        {
        private:
            double m_lower, m_upper;
        public:
            Uniform() = delete;
            Uniform(double lower, double upper);
            ~Uniform() = default;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_lower() const;
            double get_upper() const;
        };
    }
}

#endif