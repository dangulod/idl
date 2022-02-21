#include <idl/positions/position.h>

namespace idl
{
    Position::Position(double jtd, double notional,
        unsigned int rating, unsigned int region, 
        unsigned int sector, size_t idio_seed): 
        m_jtd(jtd), m_notional(notional),
        m_weight_dimension(rating, region, sector),
        m_idio_seed(idio_seed), m_pd(0), 
        m_recovery(std::shared_ptr<distributions::Beta>(nullptr)),
        m_weights(std::shared_ptr<Weights>(nullptr))
    { }

    Position::Position(double jtd, double notional,
        WeightsDimension w_dim, size_t idio_seed):
        m_jtd(jtd), m_notional(notional), m_weight_dimension(w_dim),
        m_idio_seed(idio_seed),
        m_pd(0), 
        m_recovery(std::shared_ptr<distributions::Beta>(nullptr)),
        m_weights(std::shared_ptr<Weights>(nullptr))
    { }

    bool Position::operator ==(const Position &rhs) const
    {
        return (this->get_jtd() == rhs.get_jtd()) &
            (this->get_notional() == rhs.get_notional()) &
            (this->get_rating() == rhs.get_rating()) &
            (this->get_region() == rhs.get_region()) &
            (this->get_sector() == rhs.get_sector());
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

    double Position::get_jtd() const
    {
        return this->m_jtd;
    }

    double Position::get_notional() const
    {
        return this->m_notional;
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

    std::shared_ptr<distributions::Beta> Position::get_recovery() const
    {
        return this->m_recovery;
    }

    void Position::set_recovery(const std::shared_ptr<distributions::Beta> value)
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

    size_t Position::get_n_splits(double div_threshold)
    {
        return (div_threshold > 0) ? 
                std::ceil(this->get_jtd() / div_threshold) : 
                1;
    }

    arma::vec Position::get_diversified_jtd(double div_threshold)
    {
        size_t n_splits = this->get_n_splits(div_threshold);

        if (n_splits == 1) return arma::vec{this->get_jtd()};

        arma::vec jtd(n_splits, arma::fill::ones);
        jtd *= this->get_jtd() / div_threshold;

        jtd.back() = std::fmod(this->get_jtd(), div_threshold) / div_threshold;

        return jtd;
    }

    arma::vec Position::get_cwi(arma::vec factors, 
                                size_t idio_id,
                                size_t n_splits)
    {
        double systematic = this->get_systematic(factors);
        return systematic + 
               this->get_weights()->get_idiosyncratic() *
               dist_normal(generator::idiosyncratic,
                           n_splits, 
                           this->get_idio_seed() + idio_id);
    }

    double Position::loss(arma::vec factors, 
                          size_t idio_id,
                          double div_threshold)
    {
        double loss(0);

        arma::vec jtd = this->get_diversified_jtd(div_threshold);

        arma::vec cwi = this->get_cwi(factors, 
                                      idio_id,
                                      jtd.size());

        arma::vec u_recovery = dist_uniform(generator::recovery,
                                            jtd.size(),
                                            this->get_idio_seed() + idio_id);

        auto it_jtd = jtd.begin();
        auto it_cwi = cwi.begin();
        auto it_rec = u_recovery.begin();

        while (it_jtd != jtd.end())
        {
            if ((*it_cwi) > this->get_PD().get_normal_inverse_pd())
            {
                loss += this->get_recovery()->quantile(*it_rec) * (*it_jtd);
            }
                it_jtd++;
                it_cwi++;
                it_rec++;
        }

        return loss;
    }

} // namespace idl
