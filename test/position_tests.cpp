#include <gtest/gtest.h>
#include <idl.h>

TEST(Position, class)
{
    idl::Position count(900, 1000, 5, 1, 3, 123456789);

    EXPECT_EQ(count.get_jtd(), 900);
    EXPECT_EQ(count.get_notional(), 1000);
    EXPECT_EQ(count.get_rating(), 5);
    EXPECT_EQ(count.get_region(), 1);
    EXPECT_EQ(count.get_sector(), 3);
    EXPECT_EQ(count.get_idio_seed(), 123456789);
}

TEST(Position, cwi)
{
    idl::Position count(900, 1000, 5, 1, 3, 123456789);
    idl::Weights weights({0.1, 0.1, 0.1});
    
    count.set_weights(std::make_shared<idl::Weights>(weights));

    arma::mat factors     = {{1, 2, 3}, {-1, -2, -3}};
    arma::vec sys_results = {0.6, -0.6};

    for (size_t ii = 0; ii < factors.n_rows; ii++)
    {
        EXPECT_DOUBLE_EQ(sys_results[ii],
                         count.get_systematic(factors.row(ii).t()));
    }

    arma::vec cwi_results = {-0.25103959237496543, -0.50807550512773014};

    for (size_t ii = 0; ii < factors.n_rows; ii++)
    {
        EXPECT_DOUBLE_EQ(cwi_results[ii],
                         count.get_cwi(factors.row(ii).t(), 0, ii));
    }
}