#ifndef DISTRIBUTIONS_HPP__
#define DISTRIBUTIONS_HPP__

#include <armadillo>
#include <idl/utils/stats.h>
#include <boost/math/special_functions/erf.hpp>

namespace idl
{
    namespace distributions
    {
        class Distribution
        {
        public:
            Distribution() = default;
            virtual ~Distribution() = default;

            virtual double cdf(double x) = 0;
            virtual double pdf(double x) = 0;
            virtual double quantile(double p) = 0;

            template<typename engine>
            double operator()(engine & e)
            {
                double numerator = e();
                double divisor = e.max();
                double p = numerator / divisor;
                
                return this->quantile(p);
            }

            double operator()(unsigned long seed)
            {
                std::mt19937_64 generator;
                generator.seed(seed);

                double numerator = generator();
                double divisor = generator.max();
                double p = numerator / divisor;

                return this->quantile(p);
            }

            arma::vec operator()(size_t n, unsigned long seed)
            {
                arma::vec rnd(n);

                std::mt19937_64 generator;
                generator.seed(seed);

                auto it_rnd = rnd.begin();
                
                while (it_rnd != rnd.end())
                {
                    double numerator = generator();
                    double divisor = generator.max();
                    double p = numerator / divisor;

                    *it_rnd = this->quantile(p);
                    it_rnd++;
                }

                return rnd;
            }
        };
    } // namespace distributions
} // namespace idl

#endif