#include <idl/distributions/beta.h>

namespace idl
{
    namespace distributions
    {
        namespace beta
        {
            double quantile(double p, double shape1, double shape2, double a, double b)
            {
                utils::isProbability(p);

                double scale = b - a;
                return boost::math::ibeta_inv(shape1, shape2, p) * scale + a;
            }
            
            arma::vec quantile(const arma::vec p, double shape1, double shape2, double a, double b)
            {
                arma::vec r = p;

                for (auto & ii: r)
                {
                    ii = quantile(ii, shape1, shape2, a, b);
                }

                return r;
            }

            double cdf(double x, double shape1, double shape2, double a, double b)
            {
                utils::isFinite(x);

                double scale = b - a;

                double dx = (x - a) / scale;

                return boost::math::ibeta(shape1, shape2, dx);;
            }

            arma::vec cdf(const arma::vec x, double shape1, double shape2, double a, double b)
            {
                arma::vec r = x;

                for (auto & ii: r)
                {
                    ii = cdf(ii, shape1, shape2, a, b);
                }

                return r;
            }

            double pdf(double x, double shape1, double shape2, double a, double b)
            {
                utils::isFinite(x);

                double scale = b - a;
                double t12   = tgamma(shape1 + shape2);
                double t1    = tgamma(shape1);
                double t2    = tgamma(shape2);

                double px;

                px = (x - a) / scale;

                px = (t12 * pow(px, shape1 - 1) * pow(1 - px, shape2 - 1)) / (t1 * t2 * scale);

                return px;
            }

            arma::vec pdf(const arma::vec x, double shape1, double shape2, double a, double b)
            {
                arma::vec r = x;

                for (auto & ii: r)
                {
                    ii = pdf(ii, shape1, shape2, a, b);
                }

                return r;
            }

            double random_s(unsigned long seed, double shape1, double shape2, double a, double b)
            {
                std::mt19937_64 generator;
                generator.seed(seed);

                double numerator = generator();
                double divisor = static_cast<double>(generator.max());
                double p = numerator / divisor;

                return quantile(p, shape1, shape2, a, b);
            }

            arma::vec random_v(size_t n, unsigned long seed, double shape1, double shape2, double a, double b)
            {
                arma::vec ale(n);
                double numerator, divisor, p;

                std::mt19937 generator;
                generator.seed(seed);

                for (size_t ii = 0; ii < n; ii++)
                {
                    numerator = generator();
                    divisor = static_cast<double>(generator.max());
                    p = numerator / divisor;

                    ale.at(ii) = quantile(p, shape1, shape2, a, b);
                }

                return ale;
            }
        } // namespace beta
    } // namespace distributions
} // namespace idl