#include <idl/distributions/log_normal.h>

namespace idl
{
    namespace distributions
    {
        LogNormal::LogNormal(double mean, double sd) : 
            m_mean(mean), m_sd(sd) 
        {
            if (std::isfinite(mean) != true) 
                throw std::invalid_argument("(LogNormal) Mean is not finite");
            if (sd <= 0 || (std::isfinite(sd) != true)) 
                throw std::invalid_argument("(LogNormal) Standard Deviation is not a finite or greater than zero");
        }

        bool LogNormal::operator ==(const LogNormal &rhs) const
        {
            return (this->get_mean() == rhs.get_mean()) &
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
            double diff = (log(x) - this->m_mean) / (this->m_sd * M_SQRT2);
            result = boost::math::erfc(-diff) / 2;
            return result;
        }

        double LogNormal::pdf(double x)
        {
            utils::isFinite(x);

            if(fabs(x - 0) < DBL_MIN) return 0;

            double exponent = log(x) - this->m_mean;
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
            result += this->m_mean;
            return exp(result);
        }

        double LogNormal::get_mean() const
        {
            return this->m_mean;
        }

        double LogNormal::get_sd() const
        {
            return this->m_sd;
        }

        pt::ptree LogNormal::to_ptree()
        {
            pt::ptree root;

            root.put("mean", this->get_mean());
            root.put("sd", this->get_sd());
            
            return root;
        }

        LogNormal LogNormal::from_ptree(const pt::ptree & value)
        {
            return LogNormal(value.find("mean")->second.get_value<double>(),
                             value.find("sd")->second.get_value<double>());
        }
    }
}