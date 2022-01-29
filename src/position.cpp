#include <idl/positions/position.h>

namespace idl
{
    Position::Position(double jtd, double notional,
        unsigned int rating, unsigned int region, 
        unsigned int sector, size_t idio_seed): 
        m_jtd(jtd), m_notional(notional),
        m_weight_dimension(rating, region, sector),
        m_idio_seed(idio_seed)
    { }

    Position::Position(double jtd, double notional,
        WeightsDimension w_dim, size_t idio_seed):
        m_jtd(jtd), m_notional(notional), m_weight_dimension(w_dim),
        m_idio_seed(idio_seed)
    { }

    bool Position::operator ==(const Position &rhs) const
    {
        return (this->get_jtd() == rhs.get_jtd()) &
            (this->get_notional() == rhs.get_notional()) &
            (this->get_rating() == rhs.get_rating()) &
            (this->get_region() == rhs.get_region()) &
            (this->get_sector() == rhs.get_sector());
    }

    pt::ptree Position::to_ptree() const
    {
        pt::ptree root = this->get_weight_dimension().to_ptree();

        root.put("jtd", this->get_jtd());
        root.put("notional", this->get_notional());
        root.put("idio_seed", this->get_idio_seed());

        return root;
    }
    
    Position Position::from_ptree(const pt::ptree & value)
    {
        WeightsDimension w_dim = WeightsDimension::from_ptree(value);

        return Position(value.find("jtd")->second.get_value<double>(),
                        value.find("notional")->second.get_value<double>(),
                        w_dim,
                        value.find("idio_seed")->second.get_value<size_t>());
    }

    double Position::get_jtd() const
    {
        return this->m_jtd;
    }

    double Position::get_notional() const
    {
        return this->m_notional;
    }

    size_t Position::get_idio_seed() const
    {
        return this->m_idio_seed;
    }

    WeightsDimension Position::get_weight_dimension() const
    {
        return this->m_weight_dimension;
    }

    unsigned int Position::get_rating() const
    {
        return this->get_weight_dimension().get_rating();
    }

    unsigned int Position::get_region() const
    {
        return this->get_weight_dimension().get_region();
    }

    unsigned int Position::get_sector() const
    {
        return this->get_weight_dimension().get_sector();
    }

} // namespace idl
