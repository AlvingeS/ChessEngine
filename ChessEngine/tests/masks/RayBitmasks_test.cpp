#include <gtest/gtest.h>
#include "ChessEngine/masks/RayBitmasks.h"

#include "ChessEngine/common.h"

namespace masks {
    class RayBitmasks : public ::testing::Test {
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

    // getNorthRay tests
    TEST_F(RayBitmasks, getNorthRay_Pos0_ShouldReturn0x0101010101010100) {
        bitmask expected = 0x0101010101010100ULL;
        bitmask actual = getNorthRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthRay_Pos7_ShouldReturn0x8080808080808000) {
        bitmask expected = 0x8080808080808000ULL;
        bitmask actual = getNorthRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthRay_Pos28_ShouldReturn0x1010101000000000) {
        bitmask expected = 0x1010101000000000ULL;
        bitmask actual = getNorthRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthRay_Pos32_ShouldReturn0x0101010000000000) {
        bitmask expected = 0x0101010000000000ULL;
        bitmask actual = getNorthRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthRay_Pos56_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthRay_Pos63_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getNorthEastRay tests

    TEST_F(RayBitmasks, getNorthEastRay_Pos0_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthEastRay_Pos7_ShouldReturn0x0102040810204000) {
        bitmask expected = 0x0102040810204000ULL;
        bitmask actual = getNorthEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthEastRay_Pos28_ShouldReturn0x0102040800000000) {
        bitmask expected = 0x0102040800000000ULL;
        bitmask actual = getNorthEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthEastRay_Pos32_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000;
        bitmask actual = getNorthEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthEastRay_Pos56_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000;
        bitmask actual = getNorthEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthEastRay_Pos63_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000;
        bitmask actual = getNorthEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getEastRay tests
    TEST_F(RayBitmasks, getEastRay_Pos0_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getEastRay_Pos7_ShouldReturn0x000000000000007F) {
        bitmask expected = 0x000000000000007FULL;
        bitmask actual = getEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getEastRay_Pos28_ShouldReturn0x000000000F000000) {
        bitmask expected = 0x000000000F000000ULL;
        bitmask actual = getEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getEastRay_Pos32_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getEastRay_Pos56_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getEastRay_Pos63_ShouldReturn0x7F000000000000000) {
        bitmask expected = 0x7F00000000000000ULL;
        bitmask actual = getEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthEastRay tests
    TEST_F(RayBitmasks, getSouthEastRay_Pos0_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthEastRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthEastRay_Pos7_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthEastRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthEastRay_Pos28_ShouldReturn0x0000000000080402) {
        bitmask expected = 0x0000000000080402ULL;
        bitmask actual = getSouthEastRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthEastRay_Pos32_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthEastRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthEastRay_Pos56_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthEastRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthEastRay_Pos63_ShouldReturn0x0040201008040201) {
        bitmask expected = 0x0040201008040201ULL;
        bitmask actual = getSouthEastRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthRay tests
    TEST_F(RayBitmasks, getSouthRay_Pos0_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthRay_Pos7_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthRay_Pos28_ShouldReturn0x0000000000101010) {
        bitmask expected = 0x0000000000101010ULL;
        bitmask actual = getSouthRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthRay_Pos32_ShouldReturn0x0000000001010101) {
        bitmask expected = 0x0000000001010101ULL;
        bitmask actual = getSouthRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthRay_Pos56_ShouldReturn0x001010101010101) {
        bitmask expected = 0x001010101010101ULL;
        bitmask actual = getSouthRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthRay_Pos63_ShouldReturn0x0080808080808080) {
        bitmask expected = 0x0080808080808080ULL;
        bitmask actual = getSouthRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getSouthWestRay tests
    TEST_F(RayBitmasks, getSouthWestRay_Pos0_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthWestRay_Pos7_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthWestRay_Pos28_ShouldReturn0x0000000000204080) {
        bitmask expected = 0x0000000000204080ULL;
        bitmask actual = getSouthWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthWestRay_Pos32_ShouldReturn0x0000000002040810) {
        bitmask expected = 0x0000000002040810ULL;
        bitmask actual = getSouthWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthWestRay_Pos56_ShouldReturn0x002040810204080) {
        bitmask expected = 0x002040810204080ULL;
        bitmask actual = getSouthWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getSouthWestRay_Pos63_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getSouthWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getWestRay tests

    TEST_F(RayBitmasks, getWestRay_Pos0_ShouldReturn0x00000000000000FE) {
        bitmask expected = 0x00000000000000FEULL;
        bitmask actual = getWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getWestRay_Pos7_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getWestRay_Pos28_ShouldReturn0x00000000E0000000) {
        bitmask expected = 0x00000000E0000000ULL;
        bitmask actual = getWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getWestRay_Pos32_ShouldReturn0x000000FE00000000) {
        bitmask expected = 0x000000FE00000000ULL;
        bitmask actual = getWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getWestRay_Pos56_ShouldReturn0xFE00000000000000) {
        bitmask expected = 0xFE00000000000000ULL;
        bitmask actual = getWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getWestRay_Pos63_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }

    // getNorthWestRay tests

    TEST_F(RayBitmasks, getNorthWestRay_Pos0_ShouldReturn0x8040201008040200) {
        bitmask expected = 0x8040201008040200ULL;
        bitmask actual = getNorthWestRay(indOne);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthWestRay_Pos7_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthWestRay(indTwo);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthWestRay_Pos28_ShouldReturn0x0080402000000000) {
        bitmask expected = 0x0080402000000000ULL;
        bitmask actual = getNorthWestRay(indThree);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthWestRay_Pos32_ShouldReturn0x0804020000000000) {
        bitmask expected = 0x0804020000000000ULL;
        bitmask actual = getNorthWestRay(indFour);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthWestRay_Pos56_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthWestRay(indFive);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(RayBitmasks, getNorthWestRay_Pos63_ShouldReturn0x0000000000000000) {
        bitmask expected = 0x0000000000000000ULL;
        bitmask actual = getNorthWestRay(indSix);
        ASSERT_EQ(expected, actual);
    }
}
