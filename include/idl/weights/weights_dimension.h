#ifndef WEIGHTS_DIMENSION_H__
#define WEIGHTS_DIMENSION_H__

#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

namespace pt = boost::property_tree;

namespace idl
{
    class WeightsDimension
    {
    private:
        unsigned int m_rating, m_region, m_sector;
    public:
        WeightsDimension() = delete;
        WeightsDimension(const unsigned int & rating, const unsigned int & region, const unsigned int & sector);
        ~WeightsDimension() = default;
        
        bool operator ==(const WeightsDimension &rhs) const;
        bool operator !=(const WeightsDimension &rhs) const;
        bool operator <=(const WeightsDimension &rhs) const;
        bool operator >=(const WeightsDimension &rhs) const;
        bool operator < (const WeightsDimension &rhs) const;
        bool operator > (const WeightsDimension &rhs) const;

        pt::ptree to_ptree();
        static WeightsDimension from_ptree(const pt::ptree & value);

        std::string to_string() const;
        static WeightsDimension from_string(const std::string value);

        unsigned int get_rating() const;
        unsigned int get_region() const;
        unsigned int get_sector() const;
    };
} // namespace idl

#endif