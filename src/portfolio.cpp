#include <idl/portfolio.h>

namespace idl
{
    Portfolio::Portfolio(Factor factor, IDLParams idlparams) :
        m_dist_scenarios(idl::distributions::Normal(0, 1)), m_factor(factor),
        m_idlparams(idlparams)
    { 
        
    }

    void Portfolio::add_position(std::string id, Position & value)
    {
        auto success = this->m_position.insert(std::make_pair(id, std::make_shared<Position>(value)));
        this->m_weights.insert(std::make_pair(id, 
                                              this->get_factor().at(value.get_weight_dimension())));
        this->m_recoveries.insert(std::make_pair(id, 
                                                 this->get_IDLParams().get_recovery("all")));
        this->m_pds.insert(std::make_pair(id, 
                                          this->get_IDLParams().get_default_probability("SOV", 
                                                                                        value.get_rating())));

        if (!success.second)
        {
            throw std::invalid_argument("(Portfolio::add_position) Key alredy exists in the Portfolio object");
        }
    }
    
    void Portfolio::add_position(std::string id, Position && value)
    {
        auto success = this->m_position.insert(std::make_pair(id, std::make_shared<Position>(value)));

        this->m_weights.insert(std::make_pair(id, 
                                              this->get_factor().at(value.get_weight_dimension())));
        this->m_recoveries.insert(std::make_pair(id, 
                                                 this->get_IDLParams().get_recovery("all")));
        this->m_pds.insert(std::make_pair(id, 
                                          this->get_IDLParams().get_default_probability("SOV", 
                                                                                        value.get_rating())));

        if (!success.second)
        {
            throw std::invalid_argument("(Portfolio::add_position) Key alredy exists in the Portfolio object");
        }
    }

    std::shared_ptr<Position> & Portfolio::operator[](const std::string id)
    {
        auto output = this->m_position.find(id);

        if (output == this->end())
        {
            throw std::out_of_range("(Portfolio[]) key does not exists");
        }

        return output->second;
    }

    pt::ptree Portfolio::to_ptree()
    {
        pt::ptree root;

        root.add_child("factor", this->get_factor().to_ptree());
        root.add_child("IDLParams", this->get_IDLParams().to_ptree());

        pt::ptree counterparties;

        for (const auto & ii: *this)
        {
            counterparties.push_back(std::make_pair(ii.first, ii.second.get()->to_ptree()));
        }

        root.add_child("counterparties", counterparties);
        
        return root;
    }
    Portfolio Portfolio::from_ptree(const pt::ptree & value)
    {
        Portfolio output(Factor::from_ptree(value.get_child("factor")),
                         IDLParams::from_ptree(value.get_child("IDLParams")));

        pt::ptree::const_assoc_iterator child = value.find("counterparties");

        if (child == value.not_found()) return output;

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("counterparties"))
        {
            output.add_position(ii.first, Position::from_ptree(ii.second));
        }

        return output;
    }

    std::map<std::string, std::shared_ptr<Position>>::iterator Portfolio::begin()
    {
        return this->m_position.begin();
    }

    std::map<std::string, std::shared_ptr<Position>>::iterator Portfolio::end()
    {
        return this->m_position.end();
    }

    std::map<std::string, std::shared_ptr<Position>>::const_iterator Portfolio::cbegin() const
    {
        return this->m_position.cbegin();
    }

    std::map<std::string, std::shared_ptr<Position>>::const_iterator Portfolio::cend() const
    {
        return this->m_position.end();
    }

    std::map<std::string, std::shared_ptr<Position>>::const_iterator Portfolio::begin() const
    {
        return this->m_position.begin();
    }

    std::map<std::string, std::shared_ptr<Position>>::const_iterator Portfolio::end() const
    {
        return this->m_position.end();
    }

    size_t Portfolio::size() const
    {
        return this->m_position.size();
    }

    size_t Portfolio::get_number_of_factors()
    {
        return this->get_factor().get_number_of_factors();
    }

    void Portfolio::to_json(const std::string file)
    {
        pt::write_json(file, this->to_ptree());
    }

    Portfolio Portfolio::from_json(const std::string file)
    {
        pt::ptree ptree;
        pt::read_json(file, ptree);

        return Portfolio::from_ptree(ptree);
    }

    Factor & Portfolio::get_factor() 
    {
        return this->m_factor;
    }

    IDLParams & Portfolio::get_IDLParams() 
    {
        return this->m_idlparams;
    }

    arma::mat Portfolio::correlation_sructure()
    {
        arma::mat m(this->size(), this->get_number_of_factors());
        size_t kk = 0;

        for (auto &it_weights: this->m_weights)
        {
            m.row(kk) = it_weights.second->t();
            kk++;
        }

        arma::mat cor_stru = (m * m.t());

        cor_stru.diag().ones();

        return cor_stru;
    }

    void Portfolio::v_rand(arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads)
    {
        while (id < n)
        {
            r->row(id) = this->m_dist_scenarios(this->get_number_of_factors(), seed + id).t();
            id += n_threads;
        }
    }

    arma::mat Portfolio::get_scenarios(size_t n, size_t seed, size_t n_threads)
    {
        arma::mat rand = arma::zeros(n, this->get_number_of_factors());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_rand, this, &rand, n, seed, it_thread, n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return rand;
    }

    arma::vec Portfolio::getCWI(arma::vec f, size_t idio_id)
    {
        arma::vec output(this->size());

        auto it_position = this->begin();
        auto it_weights = this->m_weights.begin();
        auto it_output = output.begin();

        while (it_weights != this->m_weights.end())
        {
            //Crear clase contenedora -> IDLPosition + Weights + PD + Recovery
            (*it_output) = arma::accu((*it_weights->second.get()) % f) 
                         + (it_weights->second->get_idiosyncratic() 
                         * this->m_dist_scenarios(idio_id + it_position->second->get_idio_seed()));
            // Generar aleatorios idio
            //(*it_output) =+ it_weights->second->get_idiosyncratic();
            it_position++;
            it_weights++;
            it_output++;
        }

        return output;
    }

    arma::vec Portfolio::getCWI(size_t seed, size_t idio_id)
    {
        arma::vec f = this->m_dist_scenarios(this->get_number_of_factors(), seed);
        return this->getCWI(f, idio_id);
    }

    void Portfolio::v_cwi(arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads)
    {
        while (id < n)
        {
            r->row(id) = this->getCWI(seed + id, id).t();
            id += n_threads;
        }
    }


    arma::mat Portfolio::get_CWIs(size_t n, size_t seed, size_t n_threads)
    {
        arma::mat cwi = arma::zeros(n, this->size());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_cwi, this, &cwi, n, seed, it_thread, n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return cwi;
    }
}
