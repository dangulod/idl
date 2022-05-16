#include <idl/distributions/log_normal.h>

namespace idl
{
    namespace distributions
    {
        LogNormal::LogNormal(double mu, double sd) : 
            m_mu(mu), m_sd(sd) 
        {
            if (std::isfinite(mu) != true) 
                throw std::invalid_argument("(LogNormal) Mean is not finite");
            if (sd <= 0 || (std::isfinite(sd) != true)) 
                throw std::invalid_argument("(LogNormal) Standard Deviation is not a finite or greater than zero");
        }

        bool LogNormal::operator ==(const LogNormal &rhs) const
        {
            return (this->get_mu() == rhs.get_mu()) &
                (this->get_sd() == rhs.get_sd());
        }

        bool LogNormal::operator !=(const LogNormal &rhs) const
        {
            return !((*this) == rhs);
        }

        double LogNormal::cdf(double x)
        {
            utils::isFinite(x);

            double result;
            double diff = (log(x) - this->m_mu) / (this->m_sd * M_SQRT2);
            result = boost::math::erfc(-diff) / 2;
            return result;
        }

        double LogNormal::pdf(double x)
        {
            utils::isFinite(x);

            if(fabs(x - 0) < DBL_MIN) return 0;

            double exponent = log(x) - this->m_mu;
            exponent *= -exponent;
            exponent /= 2 * this->m_sd * this->m_sd;

            double result = exp(exponent);
            result /= this->m_sd * sqrt(2 * M_PI) * x;

            return result;
        }

        double LogNormal::quantile(double p)
        {
            utils::isProbability(p);

            double result;

            if (fabs(p - 0) < DBL_MIN) return 0;
            if (fabs(p - 1) < DBL_MIN) return static_cast<double>(INFINITY);

            result = boost::math::erfc_inv(2 * p);

            result = -result;
            result *= this->m_sd * M_SQRT2;
            result += this->m_mu;
            return exp(result);
        }

        double LogNormal::get_mu() const
        {
            return this->m_mu;
        }

        double LogNormal::get_sd() const
        {
            return this->m_sd;
        }

        pt::ptree LogNormal::to_ptree()
        {
            pt::ptree root;

            root.put("mu", this->get_mu());
            root.put("sd", this->get_sd());
            
            return root;
        }

        double LogNormal::mean()
        {
            return exp(this->m_mu + pow(this->m_sd, 2) / 2);
        }

        LogNormal LogNormal::from_ptree(const pt::ptree & value)
        {
            return LogNormal(value.find("mu")->second.get_value<double>(),
                             value.find("sd")->second.get_value<double>());
        }
    }
}