#include <gtest/gtest.h>
#include "ChessEngine/masks/PawnBitMasks.h"

#include "ChessEngine/common.h"

namespace masks {
    class PawnBitMasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_8asWhite_ShouldReturn0x0000000001030000) {
        U64 expectedCapture = 0x0000000000020000ULL;
        U64 expectedStraight = 0x0000000001010000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(8, true);
        U64 actualStraight = getStraightPawnMovesBitmask(8, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_12asWhite_ShouldReturn0x0000000010380000) {
        U64 expectedStraight = 0x0000000010100000ULL;
        U64 expectedCapture = 0x0000000000280000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(12, true);
        U64 actualStraight = getStraightPawnMovesBitmask(12, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_15asWhite_ShouldReturn0x0000000080C00000) {
        U64 expectedCapture = 0x0000000000400000ULL;
        U64 expectedStraight = 0x0000000080800000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(15, true);
        U64 actualStraight = getStraightPawnMovesBitmask(15, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_20asWhite_ShouldReturn0x000000038000000) {
        U64 expectedCapture = 0x0000000028000000ULL;
        U64 expectedStraight = 0x0000000010000000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(20, true);
        U64 actualStraight = getStraightPawnMovesBitmask(20, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_48asBlack_ShouldReturn0x000030100000000) {
        U64 expectedCapture = 0x0000020000000000ULL;
        U64 expectedStraight = 0x0000010100000000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(48, false);
        U64 actualStraight = getStraightPawnMovesBitmask(48, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_52asBlack_ShouldReturn0x0000381000000000) {
        U64 expectedCapture = 0x0000280000000000ULL;
        U64 expectedStraight = 0x0000101000000000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(52, false);
        U64 actualStraight = getStraightPawnMovesBitmask(52, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_55asBlack_ShouldReturn0x0000C08000000000) {
        U64 expectedCapture = 0x0000400000000000ULL;
        U64 expectedStraight = 0x0000808000000000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(55, false);
        U64 actualStraight = getStraightPawnMovesBitmask(55, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitMasks, getCapturePawnMovesBitmask_44asBlack_ShouldReturn0x0000003800000000) {
        U64 expectedCapture = 0x0000002800000000ULL;
        U64 expectedStraight = 0x0000001000000000ULL;
        U64 actualCapture = getCapturePawnMovesBitmask(44, false);
        U64 actualStraight = getStraightPawnMovesBitmask(44, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }
}
