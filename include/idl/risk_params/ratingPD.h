#ifndef RATING_H__
#define RATING_H__

#include <map>
#include <memory>
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
        std::map<unsigned int, std::shared_ptr<PD>> m_ratings;
    public:
        RatingPD() = default;
        ~RatingPD() = default;

        bool operator ==(const RatingPD &rhs) const;
        bool operator !=(const RatingPD &rhs) const;

        size_t size() const;

        void add(const unsigned int rating,
                 std::shared_ptr<PD> default_probability);
        
        void add(const unsigned int rating,
                 const PD default_probability);

        std::map<unsigned int, std::shared_ptr<PD>>::iterator begin();
        std::map<unsigned int, std::shared_ptr<PD>>::iterator end();
        std::map<unsigned int, std::shared_ptr<PD>>::const_iterator cbegin() const;
        std::map<unsigned int, std::shared_ptr<PD>>::const_iterator cend() const;
        std::map<unsigned int, std::shared_ptr<PD>>::const_iterator begin() const;
        std::map<unsigned int, std::shared_ptr<PD>>::const_iterator end() const;

        std::shared_ptr<PD> operator[](const unsigned int rating) const;

        pt::ptree to_ptree();
        static RatingPD from_ptree(const pt::ptree & value);
    };
} // namespace idl

#endif