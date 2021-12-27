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
}
