#include <gtest/gtest.h>

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace logic {

class RayBitmasks_test : public ::testing::Test 
{
protected:
    int indOne;
    int indTwo;
    int indThree;
    int indFour;
    int indFive;
    int indSix;
    bitmask whitePieces;
    bitmask blackPieces;

    void SetUp() override {
        indOne = 0;
        indTwo = 7;
        indThree = 28;
        indFour = 32;
        indFive = 56;
        indSix = 63;
        whitePieces = 0x000000000000FFFFULL;
        blackPieces = 0xFFFF000000000000ULL;
    }
};

// RayBitmasks::getNorthRay tests
TEST_F(RayBitmasks_test, getNorthRay_Pos0_ShouldReturn0x0101010101010100) 
{
    bitmask expected = 0x0101010101010100ULL;
    bitmask actual = RayBitmasks::getNorthRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos7_ShouldReturn0x8080808080808000) 
{
    bitmask expected = 0x8080808080808000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos28_ShouldReturn0x1010101000000000) 
{
    bitmask expected = 0x1010101000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos32_ShouldReturn0x0101010000000000) 
{
    bitmask expected = 0x0101010000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getNorthEastRay tests

TEST_F(RayBitmasks_test, getNorthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos7_ShouldReturn0x0102040810204000) 
{
    bitmask expected = 0x0102040810204000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos28_ShouldReturn0x0102040800000000) 
{
    bitmask expected = 0x0102040800000000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getEastRay tests
TEST_F(RayBitmasks_test, getEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos7_ShouldReturn0x000000000000007F) 
{
    bitmask expected = 0x000000000000007FULL;
    bitmask actual = RayBitmasks::getEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos28_ShouldReturn0x000000000F000000) 
{
    bitmask expected = 0x000000000F000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos63_ShouldReturn0x7F000000000000000) 
{
    bitmask expected = 0x7F00000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthEastRay tests
TEST_F(RayBitmasks_test, getSouthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos28_ShouldReturn0x0000000000080402) 
{
    bitmask expected = 0x0000000000080402ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos32_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos63_ShouldReturn0x0040201008040201) 
{
    bitmask expected = 0x0040201008040201ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthRay tests
TEST_F(RayBitmasks_test, getSouthRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos28_ShouldReturn0x0000000000101010) 
{
    bitmask expected = 0x0000000000101010ULL;
    bitmask actual = RayBitmasks::getSouthRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos32_ShouldReturn0x0000000001010101) 
{
    bitmask expected = 0x0000000001010101ULL;
    bitmask actual = RayBitmasks::getSouthRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos56_ShouldReturn0x001010101010101) 
{
    bitmask expected = 0x001010101010101ULL;
    bitmask actual = RayBitmasks::getSouthRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos63_ShouldReturn0x0080808080808080) 
{
    bitmask expected = 0x0080808080808080ULL;
    bitmask actual = RayBitmasks::getSouthRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthWestRay tests
TEST_F(RayBitmasks_test, getSouthWestRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos28_ShouldReturn0x0000000000204080) 
{
    bitmask expected = 0x0000000000204080ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos32_ShouldReturn0x0000000002040810) 
{
    bitmask expected = 0x0000000002040810ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos56_ShouldReturn0x002040810204080) 
{
    bitmask expected = 0x002040810204080ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos63_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getWestRay tests

TEST_F(RayBitmasks_test, getWestRay_Pos0_ShouldReturn0x00000000000000FE) 
{
    bitmask expected = 0x00000000000000FEULL;
    bitmask actual = RayBitmasks::getWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos28_ShouldReturn0x00000000E0000000) 
{
    bitmask expected = 0x00000000E0000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos32_ShouldReturn0x000000FE00000000) 
{
    bitmask expected = 0x000000FE00000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos56_ShouldReturn0xFE00000000000000) 
{
    bitmask expected = 0xFE00000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getNorthWestRay tests

TEST_F(RayBitmasks_test, getNorthWestRay_Pos0_ShouldReturn0x8040201008040200) 
{
    bitmask expected = 0x8040201008040200ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos28_ShouldReturn0x0080402000000000) 
{
    bitmask expected = 0x0080402000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos32_ShouldReturn0x0804020000000000) 
{
    bitmask expected = 0x0804020000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
