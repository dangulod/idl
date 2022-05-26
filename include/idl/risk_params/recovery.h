#ifndef RECOVERY_H__
#define RECOVERY_H__

#include <memory>
#include <cmath>

#include <idl/distributions/distribution.hpp>
#include <idl/rnd_generator.h>

namespace idl
{
    class Recovery: public std::enable_shared_from_this<Recovery>
    {
    private:
        double m_fixed_recovery;
        std::shared_ptr<distributions::Distribution> m_distribution;
    public:
        Recovery()  = delete;
        Recovery(double fixed_recovery);
        Recovery(std::shared_ptr<distributions::Distribution> distribution);
        ~Recovery() = default;

        double generate_recovery(size_t seed,
                                 size_t replenishment,
                                 bool fixed = false);
        double generate_recovery();

        bool is_stochastic() const;

        std::shared_ptr<distributions::Distribution> get_distribution();

        pt::ptree to_ptree();
        static Recovery from_ptree(const pt::ptree & value);
    };
} // namespace idl



#endif