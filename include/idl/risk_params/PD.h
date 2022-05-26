#ifndef PD_H__
#define PD_H__

#include <idl/static_distributions.h>

namespace idl
{
    class PD
    {
    private:
        double m_pd, m_n_inv_pd;
    public:
        PD() = delete;
        PD(double pd);
        ~PD() = default;

        bool operator ==(const PD &rhs) const;
        bool operator !=(const PD &rhs) const;

        double get_pd() const;
        double get_normal_inverse_pd() const;

        double get_conditional_pd(double systematic, 
                                  double weight_idio);

        double default_time(double cwi);
    };
} // namespace idl

#endif