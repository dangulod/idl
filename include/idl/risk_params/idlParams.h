#ifndef IDRPARAMS_H__
#define IDRPARAMS_H__

#include <map>
#include <memory>

#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <idl/risk_params/ratingPD.h>
#include <idl/distributions/distributions.h>

namespace idl
{
    class IDLParams
    {
    private:
        std::map<std::string, std::shared_ptr<RatingPD>> m_ratings;
        std::map<std::string, std::shared_ptr<idl::distributions::Beta>> m_recoveries;
    public:
        IDLParams() = default;
        ~IDLParams() = default;

        void add(std::string name, RatingPD rating);
        void add(std::string name, distributions::Beta dist);

        std::shared_ptr<distributions::Beta> get_recovery(std::string value);
        PD get_default_probability(std::string value, unsigned int rating);

        pt::ptree to_ptree();
        static IDLParams from_ptree(const pt::ptree & value);
    };
} // namespace idl

#endif