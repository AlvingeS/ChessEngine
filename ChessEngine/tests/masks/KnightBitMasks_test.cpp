#include <gtest/gtest.h>
#include "ChessEngine/masks/KnightBitMasks.h"

#include "ChessEngine/common.h"

namespace masks {
    class KnightBitMasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(KnightBitMasks, getKnightBitMask_0_ShouldReturn0x0000000000020400) {
        bitmask expected = 0x0000000000020400ULL;
        bitmask actual = getKnightBitMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_1_ShouldReturn0x0000000000050800) {
        bitmask expected = 0x0000000000050800ULL;
        bitmask actual = getKnightBitMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_2_ShouldReturn0x00000000000A1100) {
        bitmask expected = 0x00000000000A1100ULL;
        bitmask actual = getKnightBitMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_15_ShouldReturn0x0000000040200020) {
        bitmask expected = 0x0000000040200020ULL;
        bitmask actual = getKnightBitMask(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_20_ShouldReturn0x0000002844004428) {
        bitmask expected = 0x0000002844004428ULL;
        bitmask actual = getKnightBitMask(20);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_49_ShouldReturn0x0800080500000000) {
        bitmask expected = 0x0800080500000000ULL;
        bitmask actual = getKnightBitMask(49);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_60_ShouldReturn0x0044280000000000) {
        bitmask expected = 0x0044280000000000ULL;
        bitmask actual = getKnightBitMask(60);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_63_ShouldReturn0x20400000000000) {
        bitmask expected = 0x20400000000000ULL;
        bitmask actual = getKnightBitMask(63);
        ASSERT_EQ(expected, actual);
    }
}
