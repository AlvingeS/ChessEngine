#include <gtest/gtest.h>

#include "ChessEngine/src/utils/BitBasics.h"

namespace utils {
class BitBasics : public ::testing::Test {
protected:
    bitmask a;

    void SetUp() override {
        a = 0x000000000000FF00ULL;
    }
};

// getBit tests
TEST_F(BitBasics, getBit_A_Pos0_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = getBit(a, 0);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasics, getBit_A_Pos9_ShouldReturnTrue)
{
    bool expected = 1;
    bool actual = getBit(a, 9);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasics, getBit_A_Pos15_ShouldReturnTrue) 
{
    bool expected = 1;
    bool actual = getBit(a, 15);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasics, getBit_A_Pos16_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = getBit(a, 16);
    ASSERT_EQ(expected, actual);
}

// indexOfLSB tests
TEST_F(BitBasics, indexOfLSB_A_ShouldReturn8) 
{
    int expected = 8;
    int actual = indexOfLSB(a);
    ASSERT_EQ(expected, actual);
}

// indexOfMSB tests
TEST_F(BitBasics, indexOfMSB_A_ShouldReturn15) 
{
    int expected = 15;
    int actual = indexOfMSB(a);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasics, indexOfMSB_0_ShouldReturnNeg1) 
{
    int expected = -1;
    int actual = indexOfMSB(0ULL);
    ASSERT_EQ(expected, actual);
}

} // namespace utils
