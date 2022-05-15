#include <idl/positions/hedge.h>

namespace idl
{
    Hedge::Hedge(double jtd, double notional, unsigned int rating):
        m_jtd(jtd), m_notional(notional), m_rating(rating) { }

    bool Hedge::operator ==(const Hedge &rhs) const
    {
        return (this->get_jtd() == rhs.get_jtd()) &
            (this->get_notional() == rhs.get_notional()) &
            (this->get_rating() == rhs.get_rating());
    }

    double Hedge::get_jtd() const
    {
        return this->m_jtd;
    }

    double Hedge::get_notional() const
    {
        return this->m_notional;
    }

    unsigned int Hedge::get_rating() const
    {
        return this->m_rating;
    }

} // namespace idl