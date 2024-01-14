#include <gtest/gtest.h>
#include "../tools/tools.h"  // Adjust the path if necessary

TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(tools::add(2, 3), 5);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(tools::add(-2, -3), -5);
}

TEST(SubTest, PositiveNumbers) {
    EXPECT_EQ(tools::sub(2, 3), -1);
}
