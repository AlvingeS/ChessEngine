#include <gtest/gtest.h>

#include "logic/movegen/utils/BitBasics.h"

namespace logic {

class BitBasicsTest : public ::testing::Test {
protected:
    bitmask a;

    void SetUp() override {
        a = 0x000000000000FF00ULL;
    }
};

// BitBasics::getBit tests
TEST_F(BitBasicsTest, getBit_A_Pos0_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::getBit(a, 0);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos9_ShouldReturnTrue)
{
    bool expected = 1;
    bool actual = BitBasics::getBit(a, 9);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos15_ShouldReturnTrue) 
{
    bool expected = 1;
    bool actual = BitBasics::getBit(a, 15);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos16_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::getBit(a, 16);
    ASSERT_EQ(expected, actual);
}

// BitBasics::indexOfLSB tests
TEST_F(BitBasicsTest, indexOfLSB_A_ShouldReturn8) 
{
    int expected = 8;
    int actual = BitBasics::indexOfLSB(a);
    ASSERT_EQ(expected, actual);
}

// BitBasics::indexOfMSB tests
TEST_F(BitBasicsTest, indexOfMSB_A_ShouldReturn15) 
{
    int expected = 15;
    int actual = BitBasics::indexOfMSB(a);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, indexOfMSB_0_ShouldReturnNeg1) 
{
    int expected = -1;
    int actual = BitBasics::indexOfMSB(0ULL);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
