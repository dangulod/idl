#include <gtest/gtest.h>
#include <idl.h>

TEST(PD, class)
{
    idl::PD pd(0.08);

    EXPECT_EQ(pd.get_pd(),
              0.08);

    EXPECT_EQ(pd.get_normal_inverse_pd(),
              -1.4050715603096327);

    EXPECT_EQ(pd.default_time(2.5366804910688523),
              0.06729499683837614);

}
