#ifndef PORFTOLIO_H__
#define PORFTOLIO_H__

#include <vector>
#include <thread>
#include <idl/factor/factor.h>
#include <idl/counterparty.h>
#include <idl/distributions/normal.h>

namespace idl
{
    class Portfolio
    {
    private:
        Factor m_factor;
        std::vector<Counterparty> m_counterparty;

        void v_rand(arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads);

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

        Counterparty & operator[](const size_t index);
        // bool operator ==(const Portfolio &rhs) const;

        std::vector<Counterparty>::iterator begin();
        std::vector<Counterparty>::iterator end();
        std::vector<Counterparty>::const_iterator cbegin() const;
        std::vector<Counterparty>::const_iterator cend() const;
        std::vector<Counterparty>::const_iterator begin() const;
        std::vector<Counterparty>::const_iterator end() const;

        size_t size() const;
        size_t get_number_of_factors();

        pt::ptree to_ptree();
        static Portfolio from_ptree(const pt::ptree & value);

        void to_json(const std::string file);
        static Portfolio from_json(const std::string file);

        Factor & get_factor();

        arma::mat get_scenarios(size_t n, size_t seed, size_t n_threads = std::thread::hardware_concurrency());
    };
    
} // namespace idl

#endif