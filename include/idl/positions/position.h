#ifndef POSITION_H__
#define POSITION_H__

#include <idl/weights/weights_dimension.h>
#include <idl/risk_params/PD.h>
#include <idl/risk_params/recovery.h>
#include <idl/weights/weights.h>
#include <idl/rnd_generator.h>
#include <idl/positions/hedge.h>

namespace idl
{
    class Position: public std::enable_shared_from_this<Position>
    {
    private:
        double m_jtd, m_jtd_unhedged, m_notional, m_notional_unhedged;
        size_t m_idio_seed;
        WeightsDimension m_weight_dimension;

        std::vector<std::shared_ptr<Hedge>> m_hedges;

        // Set when added to a portfolio
        std::shared_ptr<PD> m_pd;
        std::shared_ptr<Recovery> m_recovery;
        std::shared_ptr<Weights> m_weights;

    public:
        Position() = delete;
        Position(double jtd, double notional, unsigned int rating, unsigned int region, unsigned int sector,
                 size_t idio_seed, std::vector<std::shared_ptr<Hedge>> hedges = {});
        Position(double jtd, double notional, WeightsDimension w_dim, size_t idio_seed,
                 std::vector<std::shared_ptr<Hedge>> hedges = {});
        ~Position() = default;

        bool operator ==(const Position &rhs) const;

        Position operator+(std::shared_ptr<Hedge> &value);
        void operator+=(std::shared_ptr<Hedge> &value);

        pt::ptree to_ptree() const;
        static Position from_ptree(const pt::ptree & value);

        double get_jtd(bool hedged = true) const;
        double get_notional(bool hedged = true) const;
        size_t get_idio_seed() const;
        WeightsDimension get_weight_dimension() const;
        unsigned int get_rating() const;
        unsigned int get_region() const;
        unsigned int get_sector() const;

        std::vector<std::shared_ptr<Hedge>> get_hedges() const;

        std::shared_ptr<PD> get_PD() const;
        void set_PD(const std::shared_ptr<PD> value);

        std::shared_ptr<Recovery> get_recovery() const;
        void set_recovery(const std::shared_ptr<Recovery> value);
        
        void set_weights(const std::shared_ptr<Weights> value);
        std::shared_ptr<Weights> get_weights() const;
        
        double get_systematic(arma::vec factors);
        double get_cwi(arma::vec factors,
                       size_t idio_id,
                       size_t replenishment);

        arma::vec loss(arma::mat factors,
                       size_t idio_id,
                       std::vector<double> times,
                       double liquidity_horizon,
                       bool hedge = true);

        double loss(arma::mat factors,
                    size_t idio_id,
                    bool hedge = true);
        
        double loss_diversified(arma::mat factors, 
                                size_t idio_id,
                                bool hedge = true);
    };

} // namespace idl

#endif
