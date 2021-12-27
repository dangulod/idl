#include <gtest/gtest.h>
#include <idl.h>

TEST(Factor, class)
{
    idl::Factor idl_factor(0, 0, 0);

    idl_factor.add({1, 0, 0}, {0.1, 0.1, 0.2});
    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 0, 1}, {0.1, 0.1, 0.1});
    EXPECT_ANY_THROW(idl_factor.add({0, 0, 1}, {0.1, 0.1, 0.1}));
    idl_factor.add({0, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 0, 1}, {0.1, 0.1, 0.1});

    std::string file("/tmp/factor.json");

    idl_factor.to_json(file);
    idl::Factor idl_json = idl::Factor::from_json(file);

    EXPECT_EQ(idl_factor, idl_json);

    idl::Weights l_weights = *idl_factor[{1, 0, 0}].get();
    idl::Weights r_weights = idl::Weights({0.1, 0.1, 0.2});

    EXPECT_EQ(l_weights, r_weights);
}
