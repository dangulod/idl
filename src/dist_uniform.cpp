#include <idl/distributions/uniform.h>

namespace idl
{
    namespace distributions
    {
        Uniform::Uniform(double lower, double upper): 
            m_lower(lower), m_upper(upper)
        {
            if (std::isfinite(lower) != true) 
                throw std::invalid_argument("(Uniform) lower is not finite");
            if (std::isfinite(upper) != true) 
                throw std::invalid_argument("(Uniform) upper is not finite");
            if (lower >= upper) 
                throw std::invalid_argument("(Uniform) lower is greater than upper");
        }

        bool Uniform::operator ==(const Uniform &rhs) const
        {
            return (this->get_lower() == rhs.get_lower()) &
                (this->get_upper() == rhs.get_upper());
        }

        bool Uniform::operator !=(const Uniform &rhs) const
        {
            return !((*this) == rhs);
        }

        double Uniform::cdf(double x)
        {
            utils::isFinite(x);

            if (x < this->m_lower) return 0;
            if (x > this->m_upper) return 0;
            return (x - this->m_lower) / (this->m_upper - this->m_lower);
        }

        double Uniform::pdf(double x)
        {
            utils::isFinite(x);
            if (x < this->m_lower || x > this->m_upper) return 0;

            return (1 / this->m_upper - this->m_lower);
        }

        double Uniform::quantile(double p)
        {
            utils::isProbability(p);

            if (fabs(p - 0) < DBL_MIN) return this->m_lower;
            if (fabs(p - 1) < DBL_MIN) return this->m_upper;

            return p * (this->m_upper - this->m_lower) + this->m_lower;
        }

        double Uniform::get_lower() const
        {
            return this->m_lower;
        }

        double Uniform::get_upper() const
        {
            return this->m_upper;
        }

        pt::ptree Uniform::to_ptree()
        {
            pt::ptree root;

            root.put("lower", this->get_lower());
            root.put("upper", this->get_upper());
            
            return root;
        }

        Uniform Uniform::from_ptree(const pt::ptree & value)
        {
            return Uniform(value.find("lower")->second.get_value<double>(),
                           value.find("upper")->second.get_value<double>());
        }
    }
}