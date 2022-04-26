#include <idl/distributions/beta.h>

namespace idl
{
    namespace distributions
    {
        Beta::Beta(double shape1, double shape2, double a, double b) : 
            m_shape1(shape1), m_shape2(shape2), m_a(a), m_b(b) 
        { 
            if (shape1 <= 0 | std::isfinite(shape1) != true) 
                throw std::invalid_argument("(Beta) Shape1 is not finite or < 0");
            if (shape2 <= 0 | std::isfinite(shape2) != true) 
                throw std::invalid_argument("(Beta) Shape2 is not finite or < 0");
            if (std::isfinite(a) != true) 
                throw std::invalid_argument("(Beta) a is not finite");
            if (std::isfinite(b) != true) 
                throw std::invalid_argument("(Beta) b is not finite");
            if (a >= b) 
                throw std::invalid_argument("(Beta) a must not be greater than b");
        }

        bool Beta::operator ==(const Beta &rhs) const
        {
            return (this->get_shape1() == rhs.get_shape1()) &
                (this->get_shape2() == rhs.get_shape2()) & 
                (this->get_a() == rhs.get_a()) & 
                (this->get_b() == rhs.get_b());
        }

        bool Beta::operator !=(const Beta &rhs) const
        {
            return !((*this) == rhs);
        }

        double Beta::pdf(double x)
        {
            utils::isFinite(x);

            double scale = m_b - m_a;
            double t12   = tgamma(m_shape1 + m_shape2);
            double t1    = tgamma(m_shape1);
            double t2    = tgamma(m_shape2);

            double px;

            px = (x - m_a) / scale;

            px = (t12 * pow(px, m_shape1 - 1) * pow(1 - px, m_shape2 - 1)) / (t1 * t2 * scale);

            return px;
        }

        double Beta::cdf(double x)
        {
            utils::isFinite(x);

            double scale = this->m_b - this->m_a;

            double dx = (x - this->m_a) / scale;

            return boost::math::ibeta(this->m_shape1, this->m_shape2, dx);
        }

        double Beta::quantile(double p)
        {
            utils::isProbability(p);

            double scale = this->m_b - this->m_a;
            return boost::math::ibeta_inv(this->m_shape1, this->m_shape2, p) * scale + this->m_a;
        }

        double Beta::get_shape1() const
        {
            return this->m_shape1;
        }

        double Beta::get_shape2() const
        {
            return this->m_shape2;
        }

        double Beta::get_a() const
        {
            return this->m_a;
        }

        double Beta::get_b() const
        {
            return this->m_b;
        }

        double Beta::mean()
        {
            double scale = this->m_b - this->m_a;
            return this->m_shape1 / (this->m_shape2 + this->m_shape1) * scale + this->m_a;
        }

        pt::ptree Beta::to_ptree()
        {
            pt::ptree root;

            root.put("shape1", this->get_shape1());
            root.put("shape2", this->get_shape2());
            root.put("a", this->get_a());
            root.put("b", this->get_b());

            return root;
        }

        Beta Beta::from_ptree(const pt::ptree & value)
        {
            return Beta(value.find("shape1")->second.get_value<double>(),
                        value.find("shape2")->second.get_value<double>(),
                        value.find("a")->second.get_value<double>(),
                        value.find("b")->second.get_value<double>());
        }

    } // namespace distributions
} // namespace idl