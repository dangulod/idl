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
        Factor m_factor;
        IDLParams m_idlparams;
        std::map<std::string, std::shared_ptr<Position>> m_position;

        void v_rand(arma::cube *r,
                    size_t n,
                    size_t n_replenishment,
                    size_t seed,
                    size_t id,
                    size_t n_threads);
        void v_cwi(arma::cube *r,
                   size_t n,
                   size_t n_replenishment,
                   size_t seed,
                   size_t id,
                   size_t n_threads);
        void v_component_loss(arma::mat *r,
                              size_t n,
                              size_t n_replenishment,
                              size_t seed,
                              bool diversification,
                              bool hedge,
                              size_t id,
                              size_t n_threads);
        void v_component_loss_scen(arma::mat *r,
                                   std::vector<size_t> scenarios_ids,
                                   size_t n_replenishment,
                                   size_t seed,
                                   bool diversification,
                                   bool hedge,
                                   size_t id,
                                   size_t n_threads);
        void v_total_loss(arma::mat *r,
                          size_t n,
                          size_t n_replenishment,
                          size_t seed,
                          bool diversification,
                          bool hedge,
                          size_t id,
                          size_t n_threads);
        void v_total_loss_scen(arma::mat *r,
                               std::vector<size_t> scenarios_ids,
                               size_t n_replenishment,
                               size_t seed,
                               bool diversification,
                               bool hedge,
                               size_t id,
                               size_t n_threads);
        arma::vec id_component_loss(arma::mat *r,
                                    size_t n_replenishment,
                                    size_t seed,
                                    bool diversification,
                                    bool hedge,
                                    size_t id);
        double id_total_loss(arma::mat *r,
                             size_t n_replenishment,
                             size_t seed,
                             bool diversification,
                             bool hedge,
                             size_t id);

    public:
        Portfolio() = delete;
        Portfolio(Factor factor, IDLParams idl_params);
        Portfolio(const Portfolio & value) = delete;
        Portfolio(Portfolio && value) = default;
        Portfolio& operator=(const Portfolio & value) = delete;
        Portfolio& operator=(Portfolio && value) = default;
        ~Portfolio() = default;

        void add_position(std::string id, std::shared_ptr<Position> value);

        std::shared_ptr<Position> operator[](const std::string id);
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

        arma::cube get_scenarios(size_t n,
                                 size_t n_replenishment,
                                 size_t seed,
                                 size_t n_threads = std::thread::hardware_concurrency());

        arma::mat getCWI(arma::mat f,
                         size_t idio_id);
        arma::mat getCWI(size_t n_replenishment,
                         size_t seed,
                         size_t idio_id);
        arma::cube get_CWIs(size_t n,
                            size_t n_replenishment,
                            size_t seed,
                            size_t n_threads = std::thread::hardware_concurrency());

        arma::vec component_loss(arma::mat f,
                                 size_t idio_id,
                                 bool diversification = false,
                                 bool hedge = true);
        arma::mat component_loss(size_t n,
                                 size_t n_replenishment,
                                 size_t seed,
                                 bool diversification = false,
                                 bool hedge = true,
                                 size_t n_threads = std::thread::hardware_concurrency());
        arma::mat component_loss(std::vector<size_t> scenarios_ids,
                                 size_t n_replenishment,
                                 size_t seed,
                                 bool diversification = false,
                                 bool hedge = true,
                                 size_t n_threads = std::thread::hardware_concurrency());
        arma::vec total_loss(size_t n,
                             size_t n_replenishment,
                             size_t seed,
                             bool diversification = false,
                             bool hedge = true,
                             size_t n_threads = std::thread::hardware_concurrency());
        arma::vec total_loss(std::vector<size_t> scenarios_ids,
                             size_t n_replenishment,
                             size_t seed,
                             bool diversification = false,
                             bool hedge = true,
                             size_t n_threads = std::thread::hardware_concurrency());
    };
    
} // namespace idl

#endif