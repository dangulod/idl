#ifndef WEIGHTS_H__
#define WEIGHTS_H__

#include <armadillo>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

namespace pt = boost::property_tree;

namespace idl
{
    class Weights: public arma::vec
    {
    public:
        using arma::vec::vec;

        bool operator ==(const Weights &rhs) const;

        pt::ptree to_ptree();
        static Weights from_ptree(pt::ptree & value);
    };
    
} // namespace idl

#endif