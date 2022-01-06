#include <gtest/gtest.h>
#include <idl.h>

TEST(ratingPD, class)
{
    idl::RatingPD rating;

    rating.add(0, 0.1);
    rating.add(1, 0.2);
    rating.add(2, 0.3);
    rating.add(3, 0.4);
    rating.add(4, 0.5);
    rating.add(5, 0.6);
    rating.add(6, 0.7);
    rating.add(7, 0.8);

    pt::write_json("/tmp/rating.json", rating.to_ptree());
    pt::ptree pt;
    pt::read_json("/tmp/rating.json", pt);

    idl::RatingPD rating_json = idl::RatingPD::from_ptree(pt);
    
    EXPECT_EQ(rating_json, rating);
    EXPECT_EQ(rating.size(), 8);
    pt::write_json("/tmp/rating2.json", rating_json.to_ptree());
}
