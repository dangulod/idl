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
        m_idio_seed(idio_seed), m_pd(0), 
        m_recovery(std::shared_ptr<Recovery>(nullptr)),
        m_weights(std::shared_ptr<Weights>(nullptr))
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
        m_pd(0), 
        m_recovery(std::shared_ptr<Recovery>(nullptr)),
        m_weights(std::shared_ptr<Weights>(nullptr))
    {
        for (auto & it_hedges: hedges)
        {
            (*this) += it_hedges;
        }
    }

    bool Position::operator ==(const Position &rhs) const
    {
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

    PD Position::get_PD() const
    {
        return this->m_pd;
    }

    void Position::set_PD(const PD value)
    {
        if (this->m_pd != value)
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
        return arma::accu((*this->get_weights()) % factors);
    }

    double Position::get_cwi(arma::vec factors, 
                             size_t idio_id)
    {
        return this->get_systematic(factors) + 
               this->get_weights()->get_idiosyncratic() *
               static_distributions::dist_normal(generator::idiosyncratic,
                                                 this->get_idio_seed() + idio_id);
    }

    double Position::loss(arma::vec factors, 
                          size_t idio_id,
                          bool diversification,
                          bool hedge)
    {
        if (diversification)
        {
            double systematic = this->get_systematic(factors);

            double pd_c = this->get_PD().get_conditional_pd(systematic, 
                                                            this->get_weights()->get_idiosyncratic());

            return this->get_jtd(hedge) - this->get_notional(hedge) * pd_c * this->get_recovery()->generate_recovery();
        }

        double cwi = this->get_cwi(factors, 
                                   idio_id);

        if (cwi > this->get_PD().get_normal_inverse_pd()) return 0;

        double recovery = this->get_recovery()->generate_recovery(this->get_idio_seed() + idio_id);
        
        return this->get_jtd(hedge) - this->get_notional(hedge) * recovery;
    }

} // namespace idl
