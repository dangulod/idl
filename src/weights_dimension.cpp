#include <idl/weights/weights_dimension.h>

namespace idl
{
    WeightsDimension::WeightsDimension(const unsigned int & rating, 
                                       const unsigned int & region, 
                                       const unsigned int & sector) :
        m_rating(rating), m_region(region), m_sector(sector) 
    { }

    bool WeightsDimension::operator==(const WeightsDimension &rhs) const
    {
        return (this->m_rating == rhs.m_rating) &
            (this->m_region == rhs.m_region) &
            (this->m_sector == rhs.m_sector);
    }

    bool WeightsDimension::operator !=(const WeightsDimension &rhs) const
    {
        return (this->m_rating != rhs.m_rating) |
            (this->m_region != rhs.m_region) |
            (this->m_sector != rhs.m_sector);
    }

    bool WeightsDimension::operator <(const WeightsDimension &rhs) const
    {
        if (this->m_rating != rhs.m_rating) return (this->m_rating < rhs.m_rating);
        if (this->m_region != rhs.m_region) return (this->m_region < rhs.m_region);
        return (this->m_sector < rhs.m_sector);
    }

    bool WeightsDimension::operator >(const WeightsDimension &rhs) const
    {
        if (this->m_rating != rhs.m_rating) return (this->m_rating > rhs.m_rating);
        if (this->m_region != rhs.m_region) return (this->m_region > rhs.m_region);
        return (this->m_sector > rhs.m_sector);
    }

    bool WeightsDimension::operator <=(const WeightsDimension &rhs) const
    {
        if (this->m_rating != rhs.m_rating) return (this->m_rating < rhs.m_rating);
        if (this->m_region != rhs.m_region) return (this->m_region < rhs.m_region);
        return (this->m_sector <= rhs.m_sector);
    }

    bool WeightsDimension::operator >=(const WeightsDimension &rhs) const
    {
        if (this->m_rating != rhs.m_rating) return (this->m_rating > rhs.m_rating);
        if (this->m_region != rhs.m_region) return (this->m_region > rhs.m_region);
        return (this->m_sector >= rhs.m_sector);
    }

    unsigned int WeightsDimension::get_rating() const
    {
        return this->m_rating;
    }

    unsigned int WeightsDimension::get_region() const
    {
        return this->m_region;
    }

    unsigned int WeightsDimension::get_sector() const
    {
        return this->m_sector;
    }
    pt::ptree WeightsDimension::to_ptree()
    {
        pt::ptree root;
        
        root.put("rating", this->get_rating());
        root.put("region", this->get_region());
        root.put("sector", this->get_sector());
        
        return root;
    }

    WeightsDimension WeightsDimension::from_string(const std::string value)
    {
        std::vector<std::string> splitted;

        boost::algorithm::split(splitted, value, [](char c) { return c == ','; });

        return WeightsDimension(atoi(splitted[0].c_str()),
                                atoi(splitted[1].c_str()),
                                atoi(splitted[2].c_str()));
    }

    std::string WeightsDimension::to_string() const
    {
        return std::to_string(this->get_rating())
            + ","
            + std::to_string(this->get_region())
            + ","
            + std::to_string(this->get_sector()); 
    }

    WeightsDimension WeightsDimension::from_ptree(const pt::ptree & value)
    {
        return WeightsDimension(value.find("rating")->second.get_value<unsigned int>(),
                                value.find("region")->second.get_value<unsigned int>(),
                                value.find("sector")->second.get_value<unsigned int>());
    }
    
} // namespace idl
