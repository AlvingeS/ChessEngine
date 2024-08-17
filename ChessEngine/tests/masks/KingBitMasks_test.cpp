#include <gtest/gtest.h>
#include "ChessEngine/masks/KingBitMasks.h"

#include "ChessEngine/common.h"

namespace masks {
    class KingBitMasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(KingBitMasks, getKingBitMask_0_ShouldReturn0x0000000000000302) {
        bitmask expected = 0x0000000000000302ULL;
        bitmask actual = getKingBitMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_7_ShouldReturn0x000000000000C040) {
        bitmask expected = 0x000000000000C040ULL;
        bitmask actual = getKingBitMask(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_9_ShouldReturn0x0000000000070507) {
        bitmask expected = 0x0000000000070507ULL;
        bitmask actual = getKingBitMask(9);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_56_ShouldReturn0x0203000000000000) {
        bitmask expected = 0x0203000000000000ULL;
        bitmask actual = getKingBitMask(56);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_63_ShouldReturn0x40C0000000000000) {
        bitmask expected = 0x40C0000000000000ULL;
        bitmask actual = getKingBitMask(63);
        ASSERT_EQ(expected, actual);
    }
}
