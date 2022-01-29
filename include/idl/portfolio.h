#ifndef PORFTOLIO_H__
#define PORFTOLIO_H__

#include <thread>
#include <idl/factor/factor.h>
#include <idl/risk_params/idlParams.h>
#include <idl/positions/position.h>
#include <idl/distributions/normal.h>

namespace idl
{
    class Portfolio
    {
    private:
        idl::distributions::Normal m_dist_scenarios;
        Factor m_factor;
        IDLParams m_idlparams;
        std::map<std::string, std::shared_ptr<Position>> m_position;

        std::map<std::string, std::shared_ptr<Weights>> m_weights;
        std::map<std::string, std::shared_ptr<distributions::Beta>> m_recoveries;
        std::map<std::string, double> m_pds;

        void v_rand(arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads);
        void v_cwi (arma::mat *r, size_t n, size_t seed, size_t id, size_t n_threads);

    public:
        Portfolio() = delete;
        Portfolio(Factor factor, IDLParams idlparams);
        Portfolio(const Portfolio & value) = delete;
        Portfolio(Portfolio && value) = default;
        Portfolio& operator=(const Portfolio & value) = delete;
        Portfolio& operator=(Portfolio && value) = default;
        ~Portfolio() = default;

        void add_position(std::string id, Position & value);
        void add_position(std::string id, Position && value);

        std::shared_ptr<Position> & operator[](const std::string id);
        // bool operator ==(const Portfolio &rhs) const;

        std::map<std::string, std::shared_ptr<Position>>::iterator begin();
        std::map<std::string, std::shared_ptr<Position>>::iterator end();
        std::map<std::string, std::shared_ptr<Position>>::const_iterator cbegin() const;
        std::map<std::string, std::shared_ptr<Position>>::const_iterator cend() const;
        std::map<std::string, std::shared_ptr<Position>>::const_iterator begin() const;
        std::map<std::string, std::shared_ptr<Position>>::const_iterator end() const;

        size_t size() const;
        size_t get_number_of_factors();

        pt::ptree to_ptree();
        static Portfolio from_ptree(const pt::ptree & value);

        void to_json(const std::string file);
        static Portfolio from_json(const std::string file);

        Factor & get_factor();
        IDLParams & get_IDLParams();

        arma::mat correlation_sructure();

        arma::mat get_scenarios(size_t n, size_t seed, size_t n_threads = std::thread::hardware_concurrency());

        arma::vec getCWI(arma::vec f, size_t idio_id);
        arma::vec getCWI(size_t seed, size_t idio_id);
        arma::mat get_CWIs(size_t n, size_t seed, size_t n_threads = std::thread::hardware_concurrency());
    };
    
} // namespace idl

#endif