#include <idl/positions/position.h>

namespace idl
{
    Position::Position(double jtd, double notional,
        unsigned int rating, unsigned int region, 
        unsigned int sector, size_t idio_seed,
        std::vector<std::shared_ptr<Hedge>> hedges): 
        m_jtd(jtd), m_jtd_unhedged(jtd), m_notional(notional),
        m_notional_unhedged(notional),
        m_weight_dimension(rating, region, sector),
        m_idio_seed(idio_seed), m_pd(std::make_shared<PD>(PD(0))), 
        m_recovery(std::shared_ptr<Recovery>(new Recovery(0))),
        m_weights(std::shared_ptr<Weights>(new Weights({1})))
    {
        for (auto & it_hedges: hedges)
        {
            (*this) += it_hedges;
        }
    }

    Position::Position(double jtd, double notional,
        WeightsDimension w_dim, size_t idio_seed,
        std::vector<std::shared_ptr<Hedge>> hedges):
        m_jtd(jtd), m_jtd_unhedged(jtd), m_notional(notional),
        m_notional_unhedged(notional), m_weight_dimension(w_dim),
        m_idio_seed(idio_seed),
        m_pd(std::make_shared<PD>(PD(0))), 
        m_recovery(std::shared_ptr<Recovery>(new Recovery(0))),
        m_weights(std::shared_ptr<Weights>(new Weights({1})))
    {
        for (auto & it_hedges: hedges)
        {
            (*this) += it_hedges;
        }
    }

    bool Position::operator ==(const Position &rhs) const
    {
        if (this->get_hedges().size() != rhs.get_hedges().size()) return false;
        
        auto it_hedge_lhs = this->get_hedges().begin();
        auto it_hedge_rhs = rhs.get_hedges().begin();

        while (it_hedge_lhs != this->get_hedges().end())
        {
            if (it_hedge_lhs != it_hedge_rhs) return false;
            it_hedge_lhs++;
            it_hedge_rhs++;
        }

        return (this->get_jtd() == rhs.get_jtd()) &
            (this->get_notional() == rhs.get_notional()) &
            (this->get_rating() == rhs.get_rating()) &
            (this->get_region() == rhs.get_region()) &
            (this->get_sector() == rhs.get_sector());
    }

    Position Position::operator+(std::shared_ptr<Hedge> &rhs)
    {
        this->m_jtd      += rhs->get_jtd();
        this->m_notional += rhs->get_notional();
        this->m_hedges.push_back(rhs);
        return *this;
    }
    
    void Position::operator+=(std::shared_ptr<Hedge> &rhs)
    {
        this->m_jtd      += rhs->get_jtd();
        this->m_notional += rhs->get_notional();
        this->m_hedges.push_back(rhs);
    }

    pt::ptree Position::to_ptree() const
    {
        pt::ptree root = this->get_weight_dimension().to_ptree();

        root.put("jtd", this->get_jtd());
        root.put("notional", this->get_notional());
        root.put("idio_seed", this->get_idio_seed());

        return root;
    }
    
    Position Position::from_ptree(const pt::ptree & value)
    {
        WeightsDimension w_dim = WeightsDimension::from_ptree(value);

        return Position(value.find("jtd")->second.get_value<double>(),
                        value.find("notional")->second.get_value<double>(),
                        w_dim,
                        value.find("idio_seed")->second.get_value<size_t>());
    }

    double Position::get_jtd(bool hedged) const
    {
        if (hedged)
        {
            return this->m_jtd;
        }

        return this->m_jtd_unhedged;
    }

    double Position::get_notional(bool hedged) const
    {
        if (hedged)
        {
            return this->m_notional;
        }

        return this->m_notional_unhedged;
    }

    std::vector<std::shared_ptr<Hedge>> Position::get_hedges() const
    {
        return this->m_hedges;
    }

    size_t Position::get_idio_seed() const
    {
        return this->m_idio_seed;
    }

    WeightsDimension Position::get_weight_dimension() const
    {
        return this->m_weight_dimension;
    }

    unsigned int Position::get_rating() const
    {
        return this->get_weight_dimension().get_rating();
    }

    unsigned int Position::get_region() const
    {
        return this->get_weight_dimension().get_region();
    }

    unsigned int Position::get_sector() const
    {
        return this->get_weight_dimension().get_sector();
    }

    std::shared_ptr<PD> Position::get_PD() const
    {
        return this->m_pd;
    }

    void Position::set_PD(std::shared_ptr<PD> value)
    {
        if ((*this->m_pd) != (*value))
        {
            this->m_pd = value;
        }
    }

    std::shared_ptr<Recovery> Position::get_recovery() const
    {
        return this->m_recovery;
    }

    void Position::set_recovery(const std::shared_ptr<Recovery> value)
    {
        if (this->m_recovery != value)
        {
            this->m_recovery = value;
        }
    }
    
    void Position::set_weights(const std::shared_ptr<Weights> value)
    {
        if (this->m_weights != value)
        {
            this->m_weights = value;
        }
    }

    std::shared_ptr<Weights> Position::get_weights() const
    {
        return this->m_weights;
    }

    double Position::get_systematic(arma::vec factors)
    {
        return arma::accu(*this->get_weights() % factors);
    }

    double Position::get_cwi(arma::vec factors, 
                             size_t idio_id,
                             size_t replenishment)
    {
        return this->get_systematic(factors) + 
               (this->get_weights()->get_idiosyncratic() *
               static_distributions::dist_normal.random(generator::idiosyncratic,
                                                        replenishment,
                                                        this->get_idio_seed() + idio_id));
    }

    arma::vec Position::loss(arma::mat factors,
                             size_t idio_id,
                             std::vector<double> times,
                             double liquidity_horizon,
                             bool hedge)
    {
        size_t number_of_replenishment(factors.n_rows);
        arma::vec time_loss(times.size(), arma::fill::zeros);

        for (size_t replenishment = 0;
             replenishment < number_of_replenishment;
             replenishment++)
        {
            double cwi = this->get_cwi(factors.row(replenishment).t(), 
                                       idio_id,
                                       replenishment);

            if (cwi > (-this->get_PD()->get_normal_inverse_pd()))
            {
                double recovery = this->get_recovery()->generate_recovery(this->get_idio_seed() + idio_id,
                                                                          this->m_hedges.size() * replenishment);

                double loss = this->get_jtd() - this->get_notional() * recovery; // <- Meter en una funcion junto a los hedges

                if (hedge)
                {
                    auto it_hedge = this->m_hedges.begin();
                    size_t ii(1);

                    while(it_hedge != this->m_hedges.end())
                    {
                        double recovery = this->get_recovery()->generate_recovery(this->get_idio_seed() + idio_id,
                                                                                  ii + replenishment * this->m_hedges.size());

                        loss += (*it_hedge)->get_jtd() - (*it_hedge)->get_notional() * recovery;

                        it_hedge++;
                        ii++;
                    }
                }

                double default_time = (this->get_PD()->default_time(cwi) + 
                                      replenishment) * 
                                      liquidity_horizon;

                auto it_loss = time_loss.begin();
                auto it_time = times.begin();

                while (it_time != times.end())
                {
                    if (default_time < (*it_time))
                    {
                        (*it_loss) = loss;
                    }
                    
                    it_loss++;
                    it_time++;
                }
                
                return time_loss;
            }
        }

        return time_loss;
    }

    double Position::loss(arma::mat factors, 
                          size_t idio_id,
                          bool hedge)
    {
        size_t number_of_replenishment(factors.n_rows);
        
        for (size_t replenishment = 0;
             replenishment < number_of_replenishment;
             replenishment++)
        {
            double cwi = this->get_cwi(factors.row(replenishment).t(), 
                                       idio_id,
                                       replenishment);

            if (cwi < this->get_PD()->get_normal_inverse_pd())
            {
                double recovery = this->get_recovery()->generate_recovery(this->get_idio_seed() + idio_id,
                                                                          replenishment);
                double loss = this->get_jtd() - this->get_notional() * recovery;

                if (hedge)
                {
                    auto it_hedge = this->get_hedges().begin();
                    double hedge_id(0);

                    while (it_hedge != this->get_hedges().end())
                    {
                        double recovery = this->get_recovery()->generate_recovery(this->get_idio_seed() + idio_id,
                                                                                  hedge_id * number_of_replenishment + replenishment);
                        
                        loss += (*it_hedge)->get_jtd() - (*it_hedge)->get_notional() * recovery;
                    }
                    
                    return loss;
                }
            }
        }

        return 0;
    }

    double Position::loss_diversified(arma::mat factors, 
                                      size_t idio_id,
                                      bool hedge)
    {
        double output(0);
        double tmp_jtd(this->get_jtd(hedge));
        double tmp_notional(this->get_notional(hedge));
    
        for (size_t replenishment = 0; replenishment < factors.n_rows; replenishment++)
        {
            double systematic = this->get_systematic(factors.row(replenishment));

            double pd_c = this->get_PD()->get_conditional_pd(systematic,
                                                             this->get_weights()->get_idiosyncratic());

            output += tmp_jtd - tmp_notional * pd_c * this->get_recovery()->generate_recovery();

            tmp_notional -= tmp_notional * pd_c;
            tmp_notional -= tmp_notional * pd_c;
        }

        return output;
    }
} // namespace idl
