#include <gtest/gtest.h>
#include "ChessEngine/bits/KingBitMasks.h"

namespace bits {
    class KingBitMasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(KingBitMasks, getKingBitMask_0_ShouldReturn0x0000000000000302) {
        U64 expected = 0x0000000000000302ULL;
        U64 actual = getKingBitMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_7_ShouldReturn0x000000000000C040) {
        U64 expected = 0x000000000000C040ULL;
        U64 actual = getKingBitMask(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_9_ShouldReturn0x0000000000070507) {
        U64 expected = 0x0000000000070507ULL;
        U64 actual = getKingBitMask(9);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_56_ShouldReturn0x0203000000000000) {
        U64 expected = 0x0203000000000000ULL;
        U64 actual = getKingBitMask(56);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KingBitMasks, getKingBitMask_63_ShouldReturn0x40C0000000000000) {
        U64 expected = 0x40C0000000000000ULL;
        U64 actual = getKingBitMask(63);
        ASSERT_EQ(expected, actual);
    }
}
