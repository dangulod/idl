#include <idl/risk_params/idlParams.h>

namespace idl
{
    void IDLParams::add(std::string name, 
                        RatingPD rating)
    {        
        auto success = this->m_ratings.insert(std::make_pair(name, std::make_shared<RatingPD>(std::move(rating))));

        if (!success.second)
        {
            throw std::invalid_argument("(IDLParams::add) Key alredy exists in the IDLParams object");
        }
    }

    void IDLParams::add(std::string name, 
                        Recovery dist)
    {
        auto success = this->m_recoveries.insert(std::make_pair(name, std::make_shared<Recovery>(std::move(dist))));

        if (!success.second)
        {
            throw std::invalid_argument("(IDLParams::add) Key alredy exists in the IDLParams object");
        }
    }

    std::map<std::string, std::shared_ptr<RatingPD>> IDLParams::get_ratingPDs() const
    {
        return this->m_ratings;
    }
    
    std::map<std::string, std::shared_ptr<Recovery>> IDLParams::get_recoveries() const
    {
        return this->m_recoveries;
    }

    std::shared_ptr<Recovery> IDLParams::get_recovery(std::string value)
    {
        auto output = this->m_recoveries.find(value);

        if (output == this->m_recoveries.end())
        {
            throw std::out_of_range("(IDLParams::get_recovery) key does not exists");
        }

        return output->second;
    }
    
    std::shared_ptr<PD> IDLParams::get_default_probability(std::string value, unsigned int rating)
    {
        auto output = this->m_ratings.find(value);

        if (output == this->m_ratings.end())
        {
            throw std::out_of_range("(IDLParams::get_default_probability) key does not exists");
        }

        return output->second->operator[](rating);
    }

    pt::ptree IDLParams::to_ptree()
    {
        pt::ptree root, recoveries, ratingPD;

        for (const auto & ii: this->m_recoveries)
        {
            recoveries.push_back(std::make_pair(ii.first, ii.second.get()->to_ptree()));
        }

        for (const auto & ii: this->m_ratings)
        {
            ratingPD.push_back(std::make_pair(ii.first, ii.second.get()->to_ptree()));
        }

        root.add_child("recoveries", recoveries);
        root.add_child("ratingPD", ratingPD);

        return root;
    }
    
    IDLParams IDLParams::from_ptree(const pt::ptree & value)
    {
        IDLParams output;

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("recoveries"))
        {
            output.add(ii.first, idl::Recovery::from_ptree(ii.second));
        }

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("ratingPD"))
        {            
            output.add(ii.first, idl::RatingPD::from_ptree(ii.second));
        }

        return output;
    }
} // namespace idl
