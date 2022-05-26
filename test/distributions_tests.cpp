#include <gtest/gtest.h>
#include <idl.h>

TEST(Distribution, random_numbers)
{
    arma::vec output({-1.1082513307109318, -1.0966050642555387 , -0.12259248243200735, -2.0330402487120853, -0.38289687627698205,
                       1.349165217485721 , -0.07337933067371676, -1.44360513332048   ,  0.1759850308014845,  0.34574074100616453});
    arma::vec rnd = idl::static_distributions::dist_normal(idl::generator::factors,
                                                           10,
                                                           1);

    auto it_output = output.begin();
    auto it_rnd    = rnd.begin();

    while (it_output != output.end())
    {
        EXPECT_DOUBLE_EQ(*it_output,
                         *it_rnd);

        it_output++; it_rnd++;
    }

    it_output = output.begin();
    size_t ii(0);

    while (it_output != output.end())
    {
        EXPECT_DOUBLE_EQ(*it_output,
                         idl::static_distributions::dist_normal.random(idl::generator::factors,
                                                                       ii,
                                                                       1));

        it_output++; ii++;
    }
}
