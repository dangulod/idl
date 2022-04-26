#ifndef RND_GENERATOR_H__
#define RND_GENERATOR_H__

#include <random>

namespace idl
{
    namespace generator
    {
        static std::mt19937_64 factors;

        static std::mersenne_twister_engine<uint_fast64_t,
                                            64,312,156,31,0xb5026f5aa96619e9,
                                            39,0x5555555555555555,
                                            17,0x71d67fffeda60000,
                                            36,0xfff7eee000000000,
                                            43,6364136223846793005> idiosyncratic;

        static std::mersenne_twister_engine<uint_fast64_t,
                                            64,312,156,31,0xb5026f5aa96619e9,
                                            39,0x5555555555555555,
                                            17,0x71d67fffeda60000,
                                            31,0xfff7eee000000000,
                                            43,6364136223846793005> recovery;

    } // namespace generator
} // namespace idl

#endif