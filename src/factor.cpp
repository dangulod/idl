#include <idl/factor/factor.h>

namespace idl
{
    Factor::Factor(const unsigned int def_rating, 
                   const unsigned int def_region, 
                   const unsigned int def_sector,
                   Weights defautl_weights) :
        m_default(def_rating, def_region, def_sector) 
    { 
        this->add(this->get_default(), defautl_weights);
    }

    Factor::Factor(WeightsDimension default_weight, Weights defautl_weights):
        m_default(default_weight) 
    { 
        this->add(this->get_default(), defautl_weights);
    }

    bool Factor::operator ==(const Factor &rhs) const
    {
        if (this->size() != rhs.size()) return false;
        if (this->get_default() != rhs.get_default()) return false;

        auto it_this = this->begin();
        auto it_rhs  = rhs.begin();

        while (it_this != this->end())
        {
            if (it_this->first != it_rhs->first) return false;

            it_this++; it_rhs++;
        }

        return true;
    }

    bool Factor::operator !=(const Factor &rhs) const
    {
        return !((*this) == rhs);
    }

    void Factor::add(const WeightsDimension weightsDimension,
                     const Weights weights)
    {
        if (this->size() != 0)
        {
            if (this->get_number_of_factors() != weights.size())
            {
                throw std::invalid_argument("(Factor::add) Weights must have the same number of factors");
            }
        }

        auto success = this->m_weights.insert(std::make_pair(weightsDimension, std::make_shared<Weights>(weights)));

        if (!success.second & (*((*this)[weightsDimension].get())) != weights)
        {
            throw std::invalid_argument("(Factor::add) Key alredy exists in the Factor object");
        }
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::iterator Factor::begin()
    {
        return this->m_weights.begin();
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::iterator Factor::end()
    {
        return this->m_weights.end();
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator Factor::cbegin() const
    {
        return this->m_weights.cbegin();
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator Factor::cend() const
    {
        return this->m_weights.end();
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator Factor::begin() const
    {
        return this->m_weights.begin();
    }

    std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator Factor::end() const
    {
        return this->m_weights.end();
    }

    size_t Factor::size() const
    {
        return this->m_weights.size();
    }

    size_t Factor::get_number_of_factors() const
    {
        return this->begin()->second->size();
    }

    std::shared_ptr<Weights> Factor::operator[](const WeightsDimension & value)
    {
        auto output = this->m_weights.find(value);

        if (output == this->end())
        {
            throw std::out_of_range("(Factor class) key does not exists");
        }

        return output->second;
    }


    std::shared_ptr<Weights> Factor::at(const WeightsDimension & value)
    {
        auto output = this->m_weights.find(value);

        if (output == this->end())
        {
            return (*this)[this->get_default()];
        }

        return output->second;
    }

    WeightsDimension Factor::get_default() const
    {
        return this->m_default;
    }

    pt::ptree Factor::to_ptree()
    {
        pt::ptree root;
        root.add_child("default", this->get_default().to_ptree());

        pt::ptree weights;

        for (const auto & ii: *this)
        {
            weights.push_back(std::make_pair(ii.first.to_string(), ii.second.get()->to_ptree()));
        }

        root.add_child("weights", weights);

        return root;
    }
    
    Factor Factor::from_ptree(const pt::ptree & value)
    {
        WeightsDimension wd_default = WeightsDimension::from_ptree(value.get_child("default"));
        Weights w_default({0});

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("weights"))
        {
            pt::ptree weight = ii.second;
            
            if (wd_default == WeightsDimension::from_string(ii.first))
            {
                w_default = Weights::from_ptree(weight);
            }
        }

        Factor output(wd_default, w_default);

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("weights"))
        {
            pt::ptree weight = ii.second;
            output.add(WeightsDimension::from_string(ii.first), 
                       Weights::from_ptree(weight));
        }

        return output;
    }

    void Factor::to_json(const std::string file)
    {
        pt::write_json(file, this->to_ptree());
    }

    Factor Factor::from_json(const std::string file)
    {
        pt::ptree ptree;
        pt::read_json(file, ptree);

        return Factor::from_ptree(ptree);
    }
}