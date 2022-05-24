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

    arma::vec PD::get_conditional_pd(arma::vec systematic,
                                     double weight_idio)
    {
        arma::vec output(systematic.size());

        auto it_output     = output.begin();
        auto it_systematic = systematic.begin();

        while (it_output != output.end())
        {
            (*it_output) = static_distributions::dist_normal.cdf((this->get_normal_inverse_pd() - (*it_systematic)) / weight_idio);
            it_output++;
            it_systematic++;
        }

        return output;
    }
} // namespace idl

