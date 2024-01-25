#include <gtest/gtest.h>
#include "ChessEngine/bits/RayBitMasks.h"

namespace bits {
    class RayBitMasks : public ::testing::Test {
        protected:
                int indOne;
                int indTwo;
                int indThree;
                int indFour;
                int indFive;
                int indSix;
                U64 whitePieces;
                U64 blackPieces;
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

    // getNorthRay tests
    TEST_F(RayBitMasks, getNorthRay_Pos0_ShouldReturn0x0101010101010100) {
        U64 expected = 0x0101010101010100ULL;
        U64 actual = getNorthRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthRay_Pos7_ShouldReturn0x8080808080808000) {
        U64 expected = 0x8080808080808000ULL;
        U64 actual = getNorthRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthRay_Pos28_ShouldReturn0x1010101000000000) {
        U64 expected = 0x1010101000000000ULL;
        U64 actual = getNorthRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthRay_Pos32_ShouldReturn0x0101010000000000) {
        U64 expected = 0x0101010000000000ULL;
        U64 actual = getNorthRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthRay_Pos56_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthRay_Pos63_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getNorthEastRay tests

    TEST_F(RayBitMasks, getNorthEastRay_Pos0_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthEastRay_Pos7_ShouldReturn0x0102040810204000) {
        U64 expected = 0x0102040810204000ULL;
        U64 actual = getNorthEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthEastRay_Pos28_ShouldReturn0x0102040800000000) {
        U64 expected = 0x0102040800000000ULL;
        U64 actual = getNorthEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthEastRay_Pos32_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000;
        U64 actual = getNorthEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthEastRay_Pos56_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000;
        U64 actual = getNorthEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthEastRay_Pos63_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000;
        U64 actual = getNorthEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getEastRay tests
    TEST_F(RayBitMasks, getEastRay_Pos0_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEastRay_Pos7_ShouldReturn0x000000000000007F) {
        U64 expected = 0x000000000000007FULL;
        U64 actual = getEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEastRay_Pos28_ShouldReturn0x000000000F000000) {
        U64 expected = 0x000000000F000000ULL;
        U64 actual = getEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEastRay_Pos32_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEastRay_Pos56_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEastRay_Pos63_ShouldReturn0x7F000000000000000) {
        U64 expected = 0x7F00000000000000ULL;
        U64 actual = getEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthEastRay tests
    TEST_F(RayBitMasks, getSouthEastRay_Pos0_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthEastRay_Pos7_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthEastRay_Pos28_ShouldReturn0x0000000000080402) {
        U64 expected = 0x0000000000080402ULL;
        U64 actual = getSouthEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthEastRay_Pos32_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthEastRay_Pos56_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthEastRay_Pos63_ShouldReturn0x0040201008040201) {
        U64 expected = 0x0040201008040201ULL;
        U64 actual = getSouthEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthRay tests
    TEST_F(RayBitMasks, getSouthRay_Pos0_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthRay_Pos7_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthRay_Pos28_ShouldReturn0x0000000000101010) {
        U64 expected = 0x0000000000101010ULL;
        U64 actual = getSouthRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthRay_Pos32_ShouldReturn0x0000000001010101) {
        U64 expected = 0x0000000001010101ULL;
        U64 actual = getSouthRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthRay_Pos56_ShouldReturn0x001010101010101) {
        U64 expected = 0x001010101010101ULL;
        U64 actual = getSouthRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthRay_Pos63_ShouldReturn0x0080808080808080) {
        U64 expected = 0x0080808080808080ULL;
        U64 actual = getSouthRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthWestRay tests
    TEST_F(RayBitMasks, getSouthWestRay_Pos0_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthWestRay_Pos7_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthWestRay_Pos28_ShouldReturn0x0000000000204080) {
        U64 expected = 0x0000000000204080ULL;
        U64 actual = getSouthWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthWestRay_Pos32_ShouldReturn0x0000000002040810) {
        U64 expected = 0x0000000002040810ULL;
        U64 actual = getSouthWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthWestRay_Pos56_ShouldReturn0x002040810204080) {
        U64 expected = 0x002040810204080ULL;
        U64 actual = getSouthWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getSouthWestRay_Pos63_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getSouthWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getWestRay tests

    TEST_F(RayBitMasks, getWestRay_Pos0_ShouldReturn0x00000000000000FE) {
        U64 expected = 0x00000000000000FEULL;
        U64 actual = getWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getWestRay_Pos7_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getWestRay_Pos28_ShouldReturn0x00000000E0000000) {
        U64 expected = 0x00000000E0000000ULL;
        U64 actual = getWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getWestRay_Pos32_ShouldReturn0x000000FE00000000) {
        U64 expected = 0x000000FE00000000ULL;
        U64 actual = getWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getWestRay_Pos56_ShouldReturn0xFE00000000000000) {
        U64 expected = 0xFE00000000000000ULL;
        U64 actual = getWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getWestRay_Pos63_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getNorthWestRay tests

    TEST_F(RayBitMasks, getNorthWestRay_Pos0_ShouldReturn0x8040201008040200) {
        U64 expected = 0x8040201008040200ULL;
        U64 actual = getNorthWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthWestRay_Pos7_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthWestRay_Pos28_ShouldReturn0x0080402000000000) {
        U64 expected = 0x0080402000000000ULL;
        U64 actual = getNorthWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthWestRay_Pos32_ShouldReturn0x0804020000000000) {
        U64 expected = 0x0804020000000000ULL;
        U64 actual = getNorthWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthWestRay_Pos56_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getNorthWestRay_Pos63_ShouldReturn0x0000000000000000) {
        U64 expected = 0x0000000000000000ULL;
        U64 actual = getNorthWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getOccupiedSquaresBitmask_StartingPosition_ShouldReturn0xFFFF00000000FFFF) {
        U64 expected = 0xFFFF00000000FFFFULL;
        U64 actual = getOccupiedSquaresBitmask(whitePieces, blackPieces);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitMasks, getEmptySquaresBitmask_StartingPosition_ShouldReturn0x0000FFFFFFFF0000) {
        U64 expected = 0x0000FFFFFFFF0000ULL;
        U64 actual = getEmptySquaresBitmask(whitePieces, blackPieces);
        ASSERT_EQ(expected, actual);
    }
}
