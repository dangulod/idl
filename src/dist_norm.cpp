#include <idl/distributions/normal.h>

namespace idl
{
    namespace distributions
    {
        Normal::Normal(double mu, double sd): m_mu(mu), m_sd(sd)
        {
            if (std::isfinite(mu) != true) 
                throw std::invalid_argument("(Normal) Mean is not finite");
            if (sd <= 0 || (std::isfinite(sd) != true)) 
                throw std::invalid_argument("(Normal) Standard Deviation is not a finite or greater than zero");
        }

        bool Normal::operator ==(const Normal &rhs) const
        {
            return (this->get_mu() == rhs.get_mu()) &
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
            double diff = (x - this->m_mu) / (this->m_sd * M_SQRT2);
            result = boost::math::erfc(-diff) / 2;
            return result;
        }

        double Normal::pdf(double x)
        {
            utils::isFinite(x);

            double exponent(x - this->m_mu);
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
            result += this->m_mu;
            return result;
        }

        double Normal::get_mu() const
        {
            return this->m_mu;
        }

        double Normal::get_sd() const
        {
            return this->m_sd;
        }

        pt::ptree Normal::to_ptree()
        {
            pt::ptree root;

            root.put("mu", this->get_mu());
            root.put("sd", this->get_sd());
            
            return root;
        }

        double Normal::mean()
        {
            return this->get_mu();
        }

        Normal Normal::from_ptree(const pt::ptree & value)
        {
            return Normal(value.find("mu")->second.get_value<double>(),
                          value.find("sd")->second.get_value<double>());
        }

    } // namespace distributions
} // namespace idl
