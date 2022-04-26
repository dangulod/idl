#ifndef STATIC_DISTRIBUTIONS_H__
#define STATIC_DISTRIBUTIONS_H__

#include <idl/distributions/distributions.h>

namespace idl
{
    namespace static_distributions
    {
        static idl::distributions::Normal dist_normal(0, 1);
        static idl::distributions::Uniform dist_uniform(0, 1);
    }
}

#endif