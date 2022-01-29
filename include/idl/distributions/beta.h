#ifndef DIST_BETA_H__
#define DIST_BETA_H__

#include <armadillo>

#include <boost/math/special_functions/beta.hpp>

#include <idl/utils/stats.h>
#include <idl/distributions/distribution.hpp>

namespace idl
{
    namespace distributions
    {
        class Beta: public Distribution
        {
        private:
            double m_shape1, m_shape2, m_a, m_b;
        public:
            Beta() = delete;
            Beta(double shape1, double shape2, double a, double b);
            ~Beta() = default;

            bool operator ==(const Beta &rhs) const;
            bool operator !=(const Beta &rhs) const;

            double cdf(double x);
            double pdf(double x);
            double quantile(double p);

            double get_shape1() const;
            double get_shape2() const;
            double get_a() const;
            double get_b() const;

            pt::ptree to_ptree();
            static Beta from_ptree(const pt::ptree & value);
        };
    } // namespace distributions
} // namespace idl

#endif