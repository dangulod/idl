#include <idl/distributions/normal.h>

namespace idl
{
    namespace distributions
    {
        Normal::Normal(double mean, double sd): m_mean(mean), m_sd(sd)
        {
            if (std::isfinite(mean) != true) 
                throw std::invalid_argument("(Normal) Mean is not finite");
            if (sd <= 0 || (std::isfinite(sd) != true)) 
                throw std::invalid_argument("(Normal) Standard Deviation is not a finite or greater than zero");
        }

        bool Normal::operator ==(const Normal &rhs) const
        {
            return (this->get_mean() == rhs.get_mean()) &
                (this->get_sd() == rhs.get_sd());
        }

        bool Normal::operator !=(const Normal &rhs) const
        {
            return !((*this) == rhs);
        }

        double Normal::cdf(double x)
        {
            utils::isFinite(x);

            double result;
            double diff = (x - this->m_mean) / (this->m_sd * M_SQRT2);
            result = boost::math::erfc(-diff) / 2;
            return result;
        }

        double Normal::pdf(double x)
        {
            utils::isFinite(x);

            double exponent(x - this->m_mean);
            exponent *= -exponent;
            exponent /= 2 * this->m_sd * this->m_sd;

            double result(exp(exponent));
            result /= this->m_sd * sqrt(2 *  M_PI);

            return result;
        }

        double Normal::quantile(double p)
        {
            utils::isProbability(p);
            double result;

            if (fabs(p - 0) < DBL_MIN) return static_cast<double>(-INFINITY);
            if (fabs(p - 1) < DBL_MIN) return static_cast<double>(INFINITY);

            result = boost::math::erfc_inv(2 * p);

            result = -result;
            result *= this->m_sd * M_SQRT2;
            result += this->m_mean;
            return result;
        }

        double Normal::get_mean() const
        {
            return this->m_mean;
        }

        double Normal::get_sd() const
        {
            return this->m_sd;
        }

        pt::ptree Normal::to_ptree()
        {
            pt::ptree root;

            root.put("mean", this->get_mean());
            root.put("sd", this->get_sd());
            
            return root;
        }

        Normal Normal::from_ptree(const pt::ptree & value)
        {
            return Normal(value.find("mean")->second.get_value<double>(),
                          value.find("sd")->second.get_value<double>());
        }

    } // namespace distributions
} // namespace idl
