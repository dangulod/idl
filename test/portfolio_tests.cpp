#include <gtest/gtest.h>
#include <idl.h>

TEST(Portfolio, class)
{
    idl::Factor idl_factor(0, 0, 0, {0.1, 0.1, 0.1});

    idl_factor.add({1, 0, 0}, {0.1, 0.1, 0.2});
    idl_factor.add({0, 0, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 0, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 0}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({0, 1, 1}, {0.1, 0.1, 0.1});
    idl_factor.add({1, 0, 1}, {0.1, 0.1, 0.1});

    idl::IDLParams idl_params;

    idl::distributions::Beta all_dist(1, 1, 0, 1);
    std::shared_ptr<idl::distributions::Distribution> all_abs = std::make_shared<idl::distributions::Beta>(all_dist);

    idl_params.add("all", idl::Recovery(all_abs));
    idl_params.add("mortgages", idl::Recovery(0.35));
    
    idl::RatingPD sov;
    sov.add(1, 0.1);
    sov.add(2, 0.15);
    sov.add(3, 0.2);
    sov.add(4, 0.25);
    sov.add(5, 0.3);
    sov.add(6, 0.35);
    sov.add(7, 0.4);

    idl_params.add("SOV", sov);
    
    idl::Portfolio portfolio(idl_factor, idl_params);

    int ii(0);

    while(ii++ < 10)
    {
        std::shared_ptr<idl::Position> count(std::make_shared<idl::Position>(idl::Position(900, 1000, 5, 1, 3, ii)));

        portfolio.add_position(std::to_string(ii), count);
    }

    portfolio.get_scenarios(10, 2, 123456789).print();
    portfolio.get_CWIs(10, 2, 123456789, 1).print();

    EXPECT_EQ(portfolio.size(), 10);

    portfolio.to_json("/tmp/portfolio.json");

    idl::Portfolio p = idl::Portfolio::from_json("/tmp/portfolio.json");

    p.component_loss(100, 2, 123456789, 0, 1, 1).print();
}