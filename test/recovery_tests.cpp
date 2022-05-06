#include <gtest/gtest.h>
#include <idl.h>

TEST(Recovery, class)
{
    idl::Recovery beta_rec(std::make_shared<idl::distributions::Beta>(idl::distributions::Beta(1, 1, 0, 1)));
    idl::Recovery normal_rec(std::make_shared<idl::distributions::Normal>(idl::distributions::Normal(0, 1)));
    idl::Recovery uniform_rec(std::make_shared<idl::distributions::Uniform>(idl::distributions::Uniform(0, 1)));
    
    EXPECT_EQ(beta_rec.is_stochastic(), true);
    EXPECT_EQ(beta_rec.generate_recovery(), 0.5);
    EXPECT_EQ(normal_rec.generate_recovery(), 0);
    EXPECT_EQ(uniform_rec.generate_recovery(), 0.5);
}
