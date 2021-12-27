#include <gtest/gtest.h>
#include <idl.h>

TEST(Counterparty, class)
{
    idl::Counterparty count(1000, 5, 1, 3);

    EXPECT_EQ(count.get_jtd(), 1000);
    EXPECT_EQ(count.get_rating(), 5);
    EXPECT_EQ(count.get_region(), 1);
    EXPECT_EQ(count.get_sector(), 3);
}