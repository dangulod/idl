#ifndef RATING_H__
#define RATING_H__

#include <map>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <idl/utils/stats.h>
#include <idl/risk_params/PD.h>

namespace pt = boost::property_tree;

namespace idl
{
    class RatingPD
    {
    private:
        std::map<unsigned int, PD> m_ratings;
    public:
        RatingPD() = default;
        ~RatingPD() = default;

        bool operator ==(const RatingPD &rhs) const;
        bool operator !=(const RatingPD &rhs) const;

        size_t size() const;

        void add(const unsigned int rating,
                 const double default_probability);

        std::map<unsigned int, PD>::iterator begin();
        std::map<unsigned int, PD>::iterator end();
        std::map<unsigned int, PD>::const_iterator cbegin() const;
        std::map<unsigned int, PD>::const_iterator cend() const;
        std::map<unsigned int, PD>::const_iterator begin() const;
        std::map<unsigned int, PD>::const_iterator end() const;

        PD operator[](const unsigned int rating) const;

        pt::ptree to_ptree();
        static RatingPD from_ptree(const pt::ptree & value);
    };
} // namespace idl

#endif