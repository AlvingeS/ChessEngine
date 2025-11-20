#include <gtest/gtest.h>

#include "logic/movegen/utils/bit_basics.h"

namespace logic {

class BitBasicsTest : public ::testing::Test {
protected:
    bitmask mask;

    void SetUp() override {
        mask = 0x000000000000FF00ULL;
    }
};

// BitBasics::get_bit tests
TEST_F(BitBasicsTest, getBit_A_Pos0_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::get_bit(mask, 0);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos9_ShouldReturnTrue)
{
    bool expected = 1;
    bool actual = BitBasics::get_bit(mask, 9);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos15_ShouldReturnTrue) 
{
    bool expected = 1;
    bool actual = BitBasics::get_bit(mask, 15);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos16_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::get_bit(mask, 16);
    ASSERT_EQ(expected, actual);
}

// BitBasics::lsb_idx tests
TEST_F(BitBasicsTest, indexOfLSB_A_ShouldReturn8) 
{
    int expected = 8;
    int actual = BitBasics::lsb_idx(mask);
    ASSERT_EQ(expected, actual);
}

// BitBasics::msb_idx tests
TEST_F(BitBasicsTest, indexOfMSB_A_ShouldReturn15) 
{
    int expected = 15;
    int actual = BitBasics::msb_idx(mask);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, indexOfMSB_0_ShouldReturnNeg1) 
{
    int expected = -1;
    int actual = BitBasics::msb_idx(0ULL);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
