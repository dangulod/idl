#ifndef FACTOR_H__
#define FACTOR_H__

#include <memory>
#include <vector>
#include <boost/property_tree/json_parser.hpp>
#include <idl/weights/weights.h>
#include <idl/weights/weights_dimension.h>

namespace idl
{
    class Factor
    {
    private:
        WeightsDimension m_default;
        std::map<WeightsDimension, std::shared_ptr<Weights>> m_weights;
    public:
        Factor() = delete;
        Factor(int def_rating, int def_region, int def_sector);
        Factor(WeightsDimension default_weight);
        Factor(const Factor & value) = default;
        Factor(Factor && value) = default;
        Factor& operator=(const Factor & value) = delete;
        Factor& operator=(Factor && value) = default;
        ~Factor() = default;

        bool operator ==(const Factor &rhs) const;

        void add(const WeightsDimension dimension,
                 const Weights weights);

        std::map<WeightsDimension, std::shared_ptr<Weights>>::iterator begin();
        std::map<WeightsDimension, std::shared_ptr<Weights>>::iterator end();
        std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator cbegin() const;
        std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator cend() const;
        std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator begin() const;
        std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator end() const;

        size_t size() const;

        std::shared_ptr<Weights> operator[](const WeightsDimension & value);
        std::map<WeightsDimension, std::shared_ptr<Weights>>::const_iterator find(const WeightsDimension & value);

        WeightsDimension get_default() const;

        pt::ptree to_ptree();
        static Factor from_ptree(const pt::ptree & value);

        void to_json(const std::string file);
        static Factor from_json(const std::string file);
    };
} // namespace idl

#endif