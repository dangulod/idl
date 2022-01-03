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
        double m_R2, m_idiosyncratic;
    public:
        Weights(std::vector<double> vec);
        Weights(arma::vec vec);
        Weights(std::initializer_list<double> vec);

        bool operator ==(const Weights &rhs) const;

        double get_R2();
        double get_idiosyncratic();

        pt::ptree to_ptree();
        static Weights from_ptree(pt::ptree & value);
    };
    
} // namespace idl

#endif