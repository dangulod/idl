#include <gtest/gtest.h>
#include <idl.h>

TEST(weights, operator)
{
    using namespace idl;

    EXPECT_GE(WeightsDimension(1, 0, 0), WeightsDimension(0, 0, 1));
    EXPECT_GT(WeightsDimension(1, 0, 0), WeightsDimension(0, 0, 1));
    
    EXPECT_EQ(WeightsDimension(0, 0, 0), WeightsDimension(0, 0, 0));
    EXPECT_EQ(WeightsDimension(3, 5, 0), WeightsDimension(3, 5, 0));
    EXPECT_NE(WeightsDimension(3, 5, 0), WeightsDimension(3, 5, 1));

    EXPECT_GT(WeightsDimension(3, 5, 2), WeightsDimension(3, 5, 1));
    EXPECT_GE(WeightsDimension(3, 5, 2), WeightsDimension(3, 5, 1));
    EXPECT_GE(WeightsDimension(3, 5, 0), WeightsDimension(3, 5, 0));

    EXPECT_GT(WeightsDimension(5, 5, 0), WeightsDimension(4, 1, 0));
    EXPECT_GT(WeightsDimension(3, 5, 0), WeightsDimension(1, 5, 1));

    EXPECT_NE(WeightsDimension(0, 0, 1), WeightsDimension(0, 1, 0));
    EXPECT_LE(WeightsDimension(0, 0, 1), WeightsDimension(0, 1, 0));
    EXPECT_LT(WeightsDimension(0, 0, 1), WeightsDimension(0, 1, 0));

    EXPECT_LE(WeightsDimension(0, 0, 1), WeightsDimension(0, 0, 1));
}
