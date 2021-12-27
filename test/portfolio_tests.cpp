#include <gtest/gtest.h>
#include <idl.h>

TEST(Portfolio, class)
{
    idl::Factor idl_factor(0, 0, 0);

    idl_factor.add({1, 0, 0}, {0.1, 0.1, 0.2});
    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 0, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 0, 1}, {0.1, 0.1, 0.1});

    idl::Portfolio portfolio(idl_factor);

    idl::Counterparty count(1000, 5, 1, 3);

    int ii(0);

    while(ii++ < 10)
    {
        portfolio + count;
    }

    EXPECT_EQ(portfolio.size(), 10);

    portfolio.to_json("/tmp/portfolio.json");
}