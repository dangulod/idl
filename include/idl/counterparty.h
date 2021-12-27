#ifndef COUNTERPARTY_H__
#define COUNTERPARTY_H__

#include <idl/weights/weights_dimension.h>
#include <memory>

namespace idl
{
    class Counterparty
    {
    private:
        double m_jtd;
        WeightsDimension m_weight_dimension;
    public:
        Counterparty() = delete;
        Counterparty(double jtd, unsigned int rating, unsigned int region, unsigned int sector);
        Counterparty(double jtd, WeightsDimension w_dim);
        ~Counterparty() = default;

        bool operator ==(const Counterparty &rhs) const;

        pt::ptree to_ptree() const;
        static Counterparty from_ptree(const pt::ptree & value);

        double get_jtd() const;
        WeightsDimension get_weight_dimension() const;
        unsigned int get_rating() const;
        unsigned int get_region() const;
        unsigned int get_sector() const;
    };
    
} // namespace idl

#endif