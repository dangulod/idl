#ifndef DISTRIBUTIONS_HPP__
#define DISTRIBUTIONS_HPP__

#include <armadillo>

#include <boost/property_tree/ptree.hpp>
#include <boost/math/special_functions/erf.hpp>

#include <idl/utils/stats.h>

namespace pt = boost::property_tree;

namespace idl
{
    namespace distributions
    {
        class Distribution: public std::enable_shared_from_this<Distribution>
        {
        public:
            Distribution() = default;
            virtual ~Distribution() = default;

            virtual double cdf(double x) = 0;
            virtual double pdf(double x) = 0;
            virtual double quantile(double p) = 0;

            virtual double mean() = 0;

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

                return (*this)(generator);
            }

            template<typename engine>
            double operator()(engine e, unsigned long seed)
            {
                e.seed(seed);

                return (*this)(e);
            }

            double operator()()
            {
                std::mt19937_64 generator;

                return (*this)(generator);
            }

            arma::vec operator()(size_t n, unsigned long seed)
            {
                std::mt19937_64 generator;

                return (*this)(generator, n, seed);
            }

            template<typename engine>
            arma::vec operator()(engine e, size_t n, unsigned long seed)
            {
                e.seed(seed);
                
                arma::vec rnd(n);

                auto it_rnd = rnd.begin();
                
                while (it_rnd != rnd.end())
                {
                    *it_rnd = (*this)(e);
                    it_rnd++;
                }

                return rnd;
            }

            template<typename engine>
            arma::mat operator()(engine e, size_t n_rows, size_t n_cols, unsigned long seed)
            {
                e.seed(seed);
                
                arma::mat rnd(n_rows, n_cols);

                auto it_rnd = rnd.begin();
                
                while (it_rnd != rnd.end())
                {
                    *it_rnd = (*this)(e);
                    it_rnd++;
                }

                return rnd;
            }
        };
    } // namespace distributions
} // namespace idl

#endif