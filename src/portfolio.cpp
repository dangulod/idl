#include <idl/portfolio.h>

namespace idl
{
    Portfolio::Portfolio(Factor factor) :
        m_factor(factor) { }

    void Portfolio::operator+(Counterparty & value)
    {
        this->m_counterparty.push_back(value);
    }

    void Portfolio::operator+(Counterparty && value)
    {
        this->m_counterparty.push_back(std::move(value));
    }

    Counterparty & Portfolio::operator[](const size_t index)
    {
        return this->m_counterparty.at(index);
    }

    pt::ptree Portfolio::to_ptree()
    {
        pt::ptree root;

        root.add_child("factor", this->get_factor().to_ptree());

        pt::ptree counterparties;

        for (const auto & ii: *this)
        {
            counterparties.push_back(std::make_pair("", ii.to_ptree()));
        }

        root.add_child("counterparties", counterparties);
        
        return root;
    }
    Portfolio Portfolio::from_ptree(const pt::ptree & value)
    {
        Portfolio output(Factor::from_ptree(value.get_child("factor")));

        pt::ptree::const_assoc_iterator child = value.find("counterparties");

        if (child == value.not_found()) return output;

        BOOST_FOREACH(const pt::ptree::value_type & ii, value.get_child("counterparties"))
        {
            pt::ptree counterparty = ii.second;
            output + Counterparty::from_ptree(counterparty);
        }

        return output;
    }

    std::vector<Counterparty>::iterator Portfolio::begin()
    {
        return this->m_counterparty.begin();
    }

    std::vector<Counterparty>::iterator Portfolio::end()
    {
        return this->m_counterparty.end();
    }

    std::vector<Counterparty>::const_iterator Portfolio::cbegin() const
    {
        return this->m_counterparty.cbegin();
    }

    std::vector<Counterparty>::const_iterator Portfolio::cend() const
    {
        return this->m_counterparty.end();
    }

    std::vector<Counterparty>::const_iterator Portfolio::begin() const
    {
        return this->m_counterparty.begin();
    }

    std::vector<Counterparty>::const_iterator Portfolio::end() const
    {
        return this->m_counterparty.end();
    }

    size_t Portfolio::size() const
    {
        return this->m_counterparty.size();
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

    void Portfolio::v_rand(arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads)
    {
        while (id < n)
        {
            r->row(id) = distributions::normal::random_v(this->get_number_of_factors(), seed + id).t();
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

}
