#ifndef POSITION_H__
#define POSITION_H__

#include <idl/weights/weights_dimension.h>

namespace idl
{
    class Position
    {
    private:
        double m_jtd, m_notional;
        size_t m_idio_seed;
        WeightsDimension m_weight_dimension;
    public:
        Position() = delete;
        Position(double jtd, double notional, unsigned int rating, unsigned int region, unsigned int sector,
                 size_t idio_seed);
        Position(double jtd, double notional, WeightsDimension w_dim, size_t idio_seed);
        ~Position() = default;

        bool operator ==(const Position &rhs) const;

        pt::ptree to_ptree() const;
        static Position from_ptree(const pt::ptree & value);

        double get_jtd() const;
        double get_notional() const;
        size_t get_idio_seed() const;
        WeightsDimension get_weight_dimension() const;
        unsigned int get_rating() const;
        unsigned int get_region() const;
        unsigned int get_sector() const;
    };
    
} // namespace idl

#endif
