#include <idl/weights/weights.h>

namespace idl
{
    Weights::Weights(std::vector<double> vec) :
        arma::vec(vec)
    {
        this->m_R2 = arma::accu(arma::pow(*this, 2));
        
        if (this->get_R2() > 1) 
        {
            throw std::invalid_argument("Invalid Weights: R2 greater than 1");
        }

        this->m_idiosyncratic = sqrt(1 - this->get_R2());
    }

    Weights::Weights(arma::vec vec) :
        arma::vec(vec)
    {
        this->m_R2 = arma::accu(arma::pow(*this, 2));
        
        if (this->get_R2() > 1) 
        {
            throw std::invalid_argument("Invalid Weights: R2 greater than 1");
        }

        this->m_idiosyncratic = sqrt(1 - this->get_R2());
    }

    Weights::Weights(std::initializer_list<double> vec):
        arma::vec(vec)
    {
        this->m_R2 = arma::accu(arma::pow(*this, 2));
        
        if (this->get_R2() > 1) 
        {
            throw std::invalid_argument("Invalid Weights: R2 greater than 1");
        }

        this->m_idiosyncratic = sqrt(1 - this->get_R2());
    }
    
    bool Weights::operator==(const Weights &rhs) const
    {
        if (this->size() != rhs.size()) return false;

        auto it_this = this->begin();
        auto it_rhs  = rhs.begin();

        while (it_this != this->end())
        {
            if ((*it_this) != (*it_rhs)) return false;

            it_this++; it_rhs++;
        }

        return true;
    }

    bool Weights::operator!=(const Weights &rhs) const
    {
        if (this->size() != rhs.size()) return true;

        auto it_this = this->begin();
        auto it_rhs  = rhs.begin();

        while (it_this != this->end())
        {
            if ((*it_this) != (*it_rhs)) return true;

            it_this++; it_rhs++;
        }

        return false;
    }

    arma::vec Weights::operator%(const arma::vec & rhs) const
    {
        arma::vec output(this->size());

        auto it_weights = this->begin();
        auto it_rhs     = rhs.begin();
        auto it_output  = output.begin();

        while (it_output != output.end())
        {
            *it_output = (*it_weights) * (*it_rhs);
            
            it_output++;
            it_rhs++;
            it_weights++;
        }

        return output;
    }

    arma::vec Weights::operator*(const arma::mat & rhs) const
    {
        return rhs * arma::vec(*this);
    }

    double Weights::get_R2()
    {
        return this->m_R2;
    }
    
    double Weights::get_idiosyncratic()
    {
        return this->m_idiosyncratic;
    }

    pt::ptree Weights::to_ptree()
    {
        pt::ptree weights;

        for (const auto & ii: *this)
        {
            pt::ptree w;
            w.put("", ii);

            weights.push_back(std::make_pair("", w));
        }

        return weights;
    }

    Weights Weights::from_ptree(pt::ptree & value)
    {
        arma::vec w(value.get_child("").size());
        auto it_w = w.begin();

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child(""))
        {
            *it_w = ii.second.get_value<double>();
            it_w++;
        }

        return Weights(w);
    }
    
} // namespace idl

