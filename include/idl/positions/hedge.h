#ifndef HEDGE_H__
#define HEDGE_H__

#include <memory>

namespace idl
{
    class Hedge: public std::enable_shared_from_this<Hedge>
    {
    private:

        unsigned int m_rating;
        double m_jtd, m_notional;

    public:
        Hedge() = delete;
        Hedge(double jtd, double notional, unsigned int rating);
        ~Hedge() = default;

        bool operator ==(const Hedge &rhs) const;

        double get_jtd() const;
        double get_notional() const;
        unsigned int get_rating() const;
    };

} // namespace idl

#endif
