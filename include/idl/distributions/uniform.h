#ifndef UNIFORM_H__
#define UNIFORM_H__

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

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

            bool operator ==(const Uniform &rhs) const;
            bool operator !=(const Uniform &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_lower() const;
            double get_upper() const;

            pt::ptree to_ptree();
            static Uniform from_ptree(const pt::ptree & value);
        };
    }
}

#endif