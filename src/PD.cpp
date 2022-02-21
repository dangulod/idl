#include <idl/risk_params/PD.h>

namespace idl
{
    PD::PD(double pd): m_pd(pd)
    {
        static idl::distributions::Normal normal_distribution(0, 1);

        this->m_n_inv_pd = normal_distribution.quantile(pd);
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

} // namespace idl

