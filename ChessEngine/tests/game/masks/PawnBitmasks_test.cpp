#include <gtest/gtest.h>

#include "ChessEngine/src/game/masks/PawnBitmasks.h"

namespace masks {
    class PawnBitmasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_8asWhite_ShouldReturn0x0000000001030000) {
        bitmask expectedCapture = 0x0000000000020000ULL;
        bitmask expectedStraight = 0x0000000001010000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(8, true);
        bitmask actualStraight = getStraightPawnMovesBitmask(8, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_12asWhite_ShouldReturn0x0000000010380000) {
        bitmask expectedStraight = 0x0000000010100000ULL;
        bitmask expectedCapture = 0x0000000000280000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(12, true);
        bitmask actualStraight = getStraightPawnMovesBitmask(12, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_15asWhite_ShouldReturn0x0000000080C00000) {
        bitmask expectedCapture = 0x0000000000400000ULL;
        bitmask expectedStraight = 0x0000000080800000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(15, true);
        bitmask actualStraight = getStraightPawnMovesBitmask(15, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_20asWhite_ShouldReturn0x000000038000000) {
        bitmask expectedCapture = 0x0000000028000000ULL;
        bitmask expectedStraight = 0x0000000010000000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(20, true);
        bitmask actualStraight = getStraightPawnMovesBitmask(20, true);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_48asBlack_ShouldReturn0x000030100000000) {
        bitmask expectedCapture = 0x0000020000000000ULL;
        bitmask expectedStraight = 0x0000010100000000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(48, false);
        bitmask actualStraight = getStraightPawnMovesBitmask(48, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_52asBlack_ShouldReturn0x0000381000000000) {
        bitmask expectedCapture = 0x0000280000000000ULL;
        bitmask expectedStraight = 0x0000101000000000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(52, false);
        bitmask actualStraight = getStraightPawnMovesBitmask(52, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_55asBlack_ShouldReturn0x0000C08000000000) {
        bitmask expectedCapture = 0x0000400000000000ULL;
        bitmask expectedStraight = 0x0000808000000000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(55, false);
        bitmask actualStraight = getStraightPawnMovesBitmask(55, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }

    TEST_F(PawnBitmasks, getCapturePawnMovesBitmask_44asBlack_ShouldReturn0x0000003800000000) {
        bitmask expectedCapture = 0x0000002800000000ULL;
        bitmask expectedStraight = 0x0000001000000000ULL;
        bitmask actualCapture = getCapturePawnMovesBitmask(44, false);
        bitmask actualStraight = getStraightPawnMovesBitmask(44, false);
        ASSERT_EQ(expectedCapture, actualCapture);
        ASSERT_EQ(expectedStraight, actualStraight);
    }
}
