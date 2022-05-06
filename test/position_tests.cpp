#include <gtest/gtest.h>
#include <idl.h>

TEST(Counterparty, class)
{
    idl::Position count(900, 1000, 5, 1, 3, 123456789);

    EXPECT_EQ(count.get_jtd(), 900);
    EXPECT_EQ(count.get_notional(), 1000);
    EXPECT_EQ(count.get_rating(), 5);
    EXPECT_EQ(count.get_region(), 1);
    EXPECT_EQ(count.get_sector(), 3);
    EXPECT_EQ(count.get_idio_seed(), 123456789);
}