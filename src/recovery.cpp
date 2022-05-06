#include <idl/risk_params/recovery.h>

namespace idl
{
    Recovery::Recovery(double fixed_recovery):
        m_fixed_recovery(fixed_recovery), m_distribution(std::shared_ptr<distributions::Distribution>(nullptr))
    { 
        if ((fixed_recovery < 0) | (fixed_recovery > 1))
        {
            throw std::out_of_range("Recovery value is not contained in [0-1]");
        }
    }

    Recovery::Recovery(std::shared_ptr<distributions::Distribution> distribution) :
        m_fixed_recovery(std::fmin(std::fmax(0, distribution->mean()), 1)), m_distribution(distribution) { }

    double Recovery::generate_recovery(size_t seed, bool fixed)
    {
        if (this->m_distribution == nullptr | fixed)
        {
            return this->generate_recovery();
        }

        return std::fmin(std::fmax(0, (*this->m_distribution)(generator::recovery, seed)), 1);
    }

    double Recovery::generate_recovery()
    {
        return this->m_fixed_recovery;
    }

    bool Recovery::is_stochastic() const
    {
        return this->m_distribution.get() != nullptr;
    }

    std::shared_ptr<distributions::Distribution> Recovery::get_distribution()
    {
        return this->m_distribution;
    }

    pt::ptree Recovery::to_ptree()
    {
        pt::ptree root;

        root.put("fixed_recovery", this->m_fixed_recovery);

        return root;
    }
    
    Recovery Recovery::from_ptree(const pt::ptree & value)
    {
        return Recovery(value.find("fixed_recovery")->second.get_value<double>());
    }

} // namespace idl
