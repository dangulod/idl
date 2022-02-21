#ifndef POSITION_H__
#define POSITION_H__

#include <idl/weights/weights_dimension.h>
#include <idl/risk_params/PD.h>
#include <idl/weights/weights.h>
#include <idl/rnd_generator.h>

namespace idl
{
    class Position
    {
    private:
        double m_jtd, m_notional;
        size_t m_idio_seed;
        WeightsDimension m_weight_dimension;

        // Set when added to a portfolio
        PD m_pd;
        std::shared_ptr<distributions::Beta> m_recovery;
        std::shared_ptr<Weights> m_weights;

    public:
        Position() = delete;
        Position(double jtd, double notional, unsigned int rating, unsigned int region, unsigned int sector,
                 size_t idio_seed);
        Position(double jtd, double notional, WeightsDimension w_dim, size_t idio_seed);
        ~Position() = default;

        bool operator ==(const Position &rhs) const;

        pt::ptree to_ptree() const;
        static Position from_ptree(const pt::ptree & value);

        double get_jtd() const;
        double get_notional() const;
        size_t get_idio_seed() const;
        WeightsDimension get_weight_dimension() const;
        unsigned int get_rating() const;
        unsigned int get_region() const;
        unsigned int get_sector() const;

        PD get_PD() const;
        void set_PD(const PD value);

        std::shared_ptr<distributions::Beta> get_recovery() const;
        void set_recovery(const std::shared_ptr<distributions::Beta> value);
        
        void set_weights(const std::shared_ptr<Weights> value);
        std::shared_ptr<Weights> get_weights() const;

        size_t get_n_splits(double div_threshold);
        arma::vec get_diversified_jtd(double div_threshold);
        
        double get_systematic(arma::vec factors);
        arma::vec get_cwi(arma::vec factors, 
                          size_t idio_id,
                          size_t n_splits = 1);

        double loss(arma::vec factors, 
                    size_t idio_id,
                    double div_threshold);
    };
    
} // namespace idl

#endif
