#include <idl/risk_params/PD.h>

namespace idl
{
    PD::PD(double pd): m_pd(pd)
    {
        this->m_n_inv_pd = static_distributions::dist_normal.quantile(pd);
    }

    bool PD::operator ==(const PD &rhs) const
    {
        return this->get_pd() == rhs.get_pd();
    }

    bool PD::operator !=(const PD &rhs) const
    {
        return this->get_pd() != rhs.get_pd();
    }

    double PD::get_pd() const
    {
        return this->m_pd;
    }
    
    double PD::get_normal_inverse_pd() const
    {
        return this->m_n_inv_pd;
    }

    double PD::get_conditional_pd(double systematic,
                                  double weight_idio)
    {
        return static_distributions::dist_normal.cdf((this->get_normal_inverse_pd() - systematic) / weight_idio);
    }

    double PD::default_time(double cwi)
    {
        return log(idl::static_distributions::dist_normal.cdf(cwi)) / log(1 - this->m_pd);
    }
} // namespace idl
