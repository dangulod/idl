#include <idl/distributions/normal.h>

namespace idl
{
    namespace distributions
    {
        namespace normal
        {
            double quantile(double p, double mean, double sd)
            {
                utils::isProbability(p);
                double result;

                if (fabs(p - 0) < DBL_MIN) return static_cast<double>(-INFINITY);
                if (fabs(p - 1) < DBL_MIN) return static_cast<double>(INFINITY);

                result = boost::math::erfc_inv(2 * p);
                
                result = -result;
                result *= sd * M_SQRT2;
                result += mean;

                return result;
            }

            arma::vec quantile(const arma::vec p, double mean, double sd)
            {
                arma::vec r = p;

                for (auto & ii: r)
                {
                    ii = quantile(ii, mean, sd);
                }

                return r;
            }

            double cdf(double x, double mean, double sd)
            {
                utils::isFinite(x);

                double result;
                double diff = (x - mean) / (sd * M_SQRT2);
                result = boost::math::erfc(-diff) / 2;
                return result;
            }

            arma::vec cdf(const arma::vec x, double mean, double sd)
            {
                arma::vec r = x;

                for (auto & ii: r)
                {
                    ii = cdf(ii, mean, sd);
                }

                return r;
            }

            double pdf(double x, double mean, double sd)
            {
                utils::isFinite(x);

                double exponent(x - mean);
                exponent *= -exponent;
                exponent /= 2 * sd * sd;

                double result(exp(exponent));
                result /= sd * sqrt(2 *  M_PI);

                return result;
            }

            arma::vec pdf(const arma::vec x, double mean, double sd)
            {
                arma::vec r = x;

                for (auto & ii: r)
                {
                    ii = pdf(ii, mean, sd);
                }

                return r;
            }

            double random_s(unsigned long seed, double mean, double sd)
            {
                std::mt19937_64 generator;
                generator.seed(seed);

                double numerator = generator();
                double divisor = static_cast<double>(generator.max());
                double p = numerator / divisor;

                return quantile(p, mean, sd);
            }

            arma::vec random_v(size_t n, unsigned long seed, double mean, double sd)
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

                    ale.at(ii) = quantile(p, mean, sd);
                }

                return ale;
            }
        } // namespace normal
    } // namespace distributions
} // namespace idl
