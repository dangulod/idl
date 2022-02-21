#include <idl/risk_params/ratingPD.h>

namespace idl
{
    bool RatingPD::operator ==(const RatingPD &rhs) const
    {
        return this->m_ratings == rhs.m_ratings;
    }

    size_t RatingPD::size() const
    {
        return this->m_ratings.size();
    }

    std::map<unsigned int, PD>::iterator RatingPD::begin()
    {
        return this->m_ratings.begin();
    }

    std::map<unsigned int, PD>::iterator RatingPD::end()
    {
        return this->m_ratings.end();
    }

    std::map<unsigned int, PD>::const_iterator RatingPD::cbegin() const
    {
        return this->m_ratings.cbegin();
    }

    std::map<unsigned int, PD>::const_iterator RatingPD::cend() const
    {
        return this->m_ratings.end();
    }

    std::map<unsigned int, PD>::const_iterator RatingPD::begin() const
    {
        return this->m_ratings.begin();
    }

    std::map<unsigned int, PD>::const_iterator RatingPD::end() const
    {
        return this->m_ratings.end();
    }


    void RatingPD::add(const unsigned int rating,
                       const double default_probability)
    {
        idl::utils::isProbability(default_probability);
        
        auto success = this->m_ratings.insert(std::make_pair(rating, PD(default_probability)));

        if (!success.second & ((*this)[rating].get_pd() != default_probability))
        {
            throw std::invalid_argument("(RatingPD::add) Key already exists in the RatingPD object");
        }
    }

    PD RatingPD::operator[](const unsigned int rating) const
    {
        auto output = this->m_ratings.find(rating);

        if (output == this->end())
        {
            throw std::out_of_range("(RatingPD class) key does not exists");
        }

        return output->second;
    }

    pt::ptree RatingPD::to_ptree()
    {
        pt::ptree root;

        pt::ptree ratings;

        for (const auto & ii: *this)
        {
            ratings.put(std::to_string(ii.first), ii.second.get_pd());
        }

        root.add_child("ratings", ratings);

        return root;
    }

    RatingPD RatingPD::from_ptree(const pt::ptree & value)
    {
        RatingPD output;
        
        pt::ptree::const_assoc_iterator child = value.find("ratings");

        if (child == value.not_found()) return output;
        
        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("ratings"))
        {
            output.add(atoi(ii.first.c_str()), 
                       ii.second.get_value<double>());
        }
        
        return output;
    }

} // namespace idl
