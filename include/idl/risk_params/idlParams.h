#ifndef IDRPARAMS_H__
#define IDRPARAMS_H__

#include <map>
#include <memory>

#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <idl/risk_params/ratingPD.h>
#include <idl/risk_params/recovery.h>

namespace idl
{
    class IDLParams
    {
    private:
        std::map<std::string, std::shared_ptr<RatingPD>> m_ratings;
        std::map<std::string, std::shared_ptr<Recovery>> m_recoveries;
    public:
        IDLParams() = default;
        ~IDLParams() = default;

        void add(std::string name, RatingPD rating);
        void add(std::string name, Recovery dist);

        std::map<std::string, std::shared_ptr<RatingPD>> get_ratingPDs() const;
        std::map<std::string, std::shared_ptr<Recovery>> get_recoveries() const;

        std::shared_ptr<Recovery> get_recovery(std::string value);
        std::shared_ptr<PD> get_default_probability(std::string value, unsigned int rating);

        pt::ptree to_ptree();
        static IDLParams from_ptree(const pt::ptree & value);
    };
} // namespace idl

#endif