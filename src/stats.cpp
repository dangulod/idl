#include <idl/utils/stats.h>

namespace idl
{
    namespace utils
    {
        void isProbability(double p)
        {
            if (p < 0 | p > 1 | !std::isfinite(p))
            {
                throw std::out_of_range("Argument is not a probabiluty, it is not contained in [0-1]");
            }
        }

        void isFinite(double value)
        {
            if (std::isfinite(value) != true)
            {
                throw std::invalid_argument("value is not finite");
            }
        }
    } // namespace utils
} // namespace idl