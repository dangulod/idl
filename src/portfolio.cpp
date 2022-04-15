#include <idl/portfolio/portfolio.h>

namespace idl
{
    Portfolio::Portfolio(Factor factor, IDLParams idlparams) :
        m_factor(factor), m_idlparams(idlparams)
    { 
        
    }

    void Portfolio::add_position(std::string id, Position & value)
    {
        value.set_PD(this->get_IDLParams().get_default_probability("SOV", 
                                                                   value.get_rating()));
        value.set_recovery(this->get_IDLParams().get_recovery("all"));
        value.set_weights(this->get_factor().at(value.get_weight_dimension()));

        auto success = this->m_position.insert(std::make_pair(id, std::make_shared<Position>(value)));

        if (!success.second)
        {
            throw std::invalid_argument("(Portfolio::add_position) Key alredy exists in the Portfolio object");
        }
    }
    
    void Portfolio::add_position(std::string id, Position && value)
    {
        value.set_PD(this->get_IDLParams().get_default_probability("SOV", 
                                                                   value.get_rating()));
        value.set_recovery(this->get_IDLParams().get_recovery("all"));
        value.set_weights(this->get_factor().at(value.get_weight_dimension()));

        auto success = this->m_position.insert(std::make_pair(id, std::make_shared<Position>(value)));

        if (!success.second)
        {
            throw std::invalid_argument("(Portfolio::add_position) Key alredy exists in the Portfolio object");
        }
    }

    Position & Portfolio::operator[](const std::string id)
    {
        auto output = this->m_position.find(id);

        if (output == this->end())
        {
            throw std::out_of_range("(Portfolio[]) key does not exists");
        }

        return *output->second.get();
    }

    pt::ptree Portfolio::to_ptree()
    {
        pt::ptree root;

        root.add_child("factor", this->get_factor().to_ptree());
        root.add_child("IDLParams", this->get_IDLParams().to_ptree());

        pt::ptree counterparties;

        for (const auto & ii: *this)
        {
            counterparties.push_back(std::make_pair(ii.first, ii.second->to_ptree()));
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

        for (auto &it_weights: this->m_position)
        {
            m.row(kk) = it_weights.second->get_weights()->t();
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
            r->row(id) = dist_normal(generator::factors, 
                                     this->get_number_of_factors(), 
                                     seed + id).t();
            id += n_threads;
        }
    }

    arma::mat Portfolio::get_scenarios(size_t n, size_t seed, size_t n_threads)
    {
        arma::mat rand = arma::zeros(n, this->get_number_of_factors());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_rand,
                                                  this,
                                                  &rand,
                                                  n,
                                                  seed,
                                                  it_thread,
                                                  n_threads);
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
        auto it_output   = output.begin();

        while (it_position != this->end())
        {
            (*it_output) = it_position->second->get_cwi(f,
                                                        idio_id,
                                                        1).at(0);
            it_position++;
            it_output++;
        }

        return output;
    }

    arma::vec Portfolio::getCWI(size_t seed, size_t idio_id)
    {
        arma::vec f = dist_normal(generator::factors, 
                                  this->get_number_of_factors(), 
                                  seed);
        return this->getCWI(f, idio_id);
    }

    void Portfolio::v_cwi(arma::mat *r,
                          size_t n,
                          size_t seed,
                          size_t id,
                          size_t n_threads)
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
            v_threads.at(it_thread) = std::thread(&Portfolio::v_cwi,
                                                  this,
                                                  &cwi,
                                                  n,
                                                  seed,
                                                  it_thread,
                                                  n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return cwi;
    }
    
    arma::vec Portfolio::component_loss(arma::vec f, 
                                        size_t idio_id, 
                                        double div_threshold)
    {
        arma::vec output(this->size());

        auto it_position = this->begin();
        auto it_output   = output.begin();
        
        while (it_position != this->end())
        {
            *it_output = it_position->second->loss(f,
                                                   idio_id,
                                                   div_threshold);
            
            it_position++;
            it_output++;
        }

        return output;
    }

    arma::vec Portfolio::id_component_loss(arma::mat *r,
                                           size_t seed,
                                           double div_threshold,
                                           size_t id)
    {
        arma::vec f = dist_normal(generator::factors, 
                                  this->get_number_of_factors(), 
                                  seed);
        return this->component_loss(f, id, div_threshold);
    }

    void Portfolio::v_component_loss(arma::mat *r,
                                     size_t n,
                                     size_t seed,
                                     double div_threshold,
                                     size_t id,
                                     size_t n_threads)
    {
        while (id < n)
        {
            r->row(id) = this->id_component_loss(r, seed, div_threshold, id).t();
            id += n_threads;
        }
    }

    void Portfolio::v_component_loss_scen(arma::mat *r, 
                                          std::vector<size_t> scenarios_ids, 
                                          size_t seed, 
                                          double div_threshold,
                                          size_t id,
                                          size_t n_threads)
    {
        while (id < scenarios_ids.size())
        {
            r->row(id) = this->id_component_loss(r, seed, div_threshold, scenarios_ids.at(id)).t();
            id += n_threads;
        }
    }

    arma::mat Portfolio::component_loss(size_t n,
                                        size_t seed,
                                        double div_threshold,
                                        size_t n_threads)
    {
        arma::mat loss = arma::zeros(n, this->size());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_component_loss,
                                                  this,
                                                  &loss,
                                                  n,
                                                  seed,
                                                  div_threshold,
                                                  it_thread,
                                                  n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return loss;
    }

    arma::mat Portfolio::component_loss(std::vector<size_t> scenarios_ids,
                                        size_t seed, 
                                        double div_threshold, 
                                        size_t n_threads)
    {
        arma::mat loss = arma::zeros(scenarios_ids.size(), this->size());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_component_loss_scen,
                                                  this,
                                                  &loss,
                                                  scenarios_ids,
                                                  seed,
                                                  div_threshold,
                                                  it_thread,
                                                  n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return loss;
    }

    double Portfolio::id_total_loss(arma::mat *r,
                                    size_t seed,
                                    double div_threshold,
                                    size_t id)
    {
        arma::vec f = dist_normal(generator::factors, 
                                      this->get_number_of_factors(), 
                                      seed);
        return arma::accu(this->component_loss(f, id, div_threshold));
    }
    
    void Portfolio::v_total_loss(arma::mat *r,
                                 size_t n,
                                 size_t seed,
                                 double div_threshold,
                                 size_t id,
                                 size_t n_threads)
    {
        while (id < n)
        {
            r->at(id) = this->id_total_loss(r, seed, div_threshold, id);
            id += n_threads;
        }
    }

    void Portfolio::v_total_loss_scen(arma::mat *r,
                                      std::vector<size_t> scenarios_ids, 
                                      size_t seed,
                                      double div_threshold,
                                      size_t id,
                                      size_t n_threads)
    {
        while (id < scenarios_ids.size())
        {
            r->at(id) = this->id_total_loss(r, seed, div_threshold, scenarios_ids.at(id));
            id += n_threads;
        }
    }

    arma::vec Portfolio::total_loss(size_t n,
                                    size_t seed,
                                    double div_threshold,
                                    size_t n_threads)
    {
        arma::vec loss = arma::zeros(n);

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_total_loss,
                                                  this,
                                                  &loss,
                                                  n,
                                                  seed,
                                                  div_threshold,
                                                  it_thread,
                                                  n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return loss;
    }

    arma::vec Portfolio::total_loss(std::vector<size_t> scenarios_ids,
                                    size_t seed,
                                    double div_threshold,
                                    size_t n_threads)
    {
        arma::vec loss = arma::zeros(scenarios_ids.size());

        std::vector<std::thread> v_threads(n_threads);

        for (size_t it_thread = 0; it_thread < n_threads; it_thread ++)
        {
            v_threads.at(it_thread) = std::thread(&Portfolio::v_total_loss_scen,
                                                  this,
                                                  &loss,
                                                  scenarios_ids,
                                                  seed,
                                                  div_threshold,
                                                  it_thread,
                                                  n_threads);
        }

        for (auto & ii: v_threads)
        {
            ii.join();
        }

        return loss;
    }
}
