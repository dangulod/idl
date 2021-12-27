#ifndef PORFTOLIO_H__
#define PORFTOLIO_H__

#include <vector>
#include <idl/factor/factor.h>
#include <idl/counterparty.h>

namespace idl
{
    class Portfolio
    {
    private:
        Factor m_factor;
        std::vector<Counterparty> m_counterparty;

    public:
        Portfolio() = delete;
        Portfolio(Factor factor);
        Portfolio(const Portfolio & value) = delete;
        Portfolio(Portfolio && value) = default;
        Portfolio& operator=(const Portfolio & value) = delete;
        Portfolio& operator=(Portfolio && value) = default;
        ~Portfolio() = default;

        void operator+(Counterparty & value);
        void operator+(Counterparty && value);

        // bool operator ==(const Portfolio &rhs) const;

        std::vector<Counterparty>::iterator begin();
        std::vector<Counterparty>::iterator end();
        std::vector<Counterparty>::const_iterator cbegin() const;
        std::vector<Counterparty>::const_iterator cend() const;
        std::vector<Counterparty>::const_iterator begin() const;
        std::vector<Counterparty>::const_iterator end() const;

        size_t size() const;

        pt::ptree to_ptree();
        static Portfolio from_ptree(const pt::ptree & value);

        void to_json(const std::string file);
        static Portfolio from_json(const std::string file);

        Factor & get_factor();
    };
    
} // namespace idl

#endif