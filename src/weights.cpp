#include <idl/weights/weights.h>

namespace idl
{
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
        Weights w(value.get_child("").size());
        auto it_w = w.begin();

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child(""))
        {
            *it_w = ii.second.get_value<double>();
            it_w++;
        }

        return w;
    }
    
} // namespace idl

