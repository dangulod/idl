#include <idl/counterparty.h>

namespace idl
{
    Counterparty::Counterparty(double jtd,  unsigned int rating, 
        unsigned int region, unsigned int sector): m_jtd(jtd), 
        m_weight_dimension(rating, region, sector)
    { }

    Counterparty::Counterparty(double jtd, WeightsDimension w_dim):
        m_jtd(jtd), m_weight_dimension(w_dim)
    { }

    bool Counterparty::operator ==(const Counterparty &rhs) const
    {
        return (this->get_jtd() == rhs.get_jtd()) &
            (this->get_rating() == rhs.get_rating()) &
            (this->get_region() == rhs.get_region()) &
            (this->get_sector() == rhs.get_sector());
    }

    pt::ptree Counterparty::to_ptree() const
    {
        pt::ptree root = this->get_weight_dimension().to_ptree();

        root.put("jtd", this->get_jtd());
        
        return root;
    }
    
    Counterparty Counterparty::from_ptree(const pt::ptree & value)
    {
        WeightsDimension w_dim = WeightsDimension::from_ptree(value);

        return Counterparty(value.find("jtd")->second.get_value<unsigned int>(),
                            w_dim);
    }

    double Counterparty::get_jtd() const
    {
        return this->m_jtd;
    }

    WeightsDimension Counterparty::get_weight_dimension() const
    {
        return this->m_weight_dimension;
    }

    unsigned int Counterparty::get_rating() const
    {
        return this->get_weight_dimension().get_rating();
    }

    unsigned int Counterparty::get_region() const
    {
        return this->get_weight_dimension().get_region();
    }

    unsigned int Counterparty::get_sector() const
    {
        return this->get_weight_dimension().get_sector();
    }

} // namespace idl
