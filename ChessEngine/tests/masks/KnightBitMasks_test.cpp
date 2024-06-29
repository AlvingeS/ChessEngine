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
        U64 expected = 0x0000000000020400ULL;
        U64 actual = getKnightBitMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_1_ShouldReturn0x0000000000050800) {
        U64 expected = 0x0000000000050800ULL;
        U64 actual = getKnightBitMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_2_ShouldReturn0x00000000000A1100) {
        U64 expected = 0x00000000000A1100ULL;
        U64 actual = getKnightBitMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_15_ShouldReturn0x0000000040200020) {
        U64 expected = 0x0000000040200020ULL;
        U64 actual = getKnightBitMask(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_20_ShouldReturn0x0000002844004428) {
        U64 expected = 0x0000002844004428ULL;
        U64 actual = getKnightBitMask(20);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_49_ShouldReturn0x0800080500000000) {
        U64 expected = 0x0800080500000000ULL;
        U64 actual = getKnightBitMask(49);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_60_ShouldReturn0x0044280000000000) {
        U64 expected = 0x0044280000000000ULL;
        U64 actual = getKnightBitMask(60);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(KnightBitMasks, getKnightBitMask_63_ShouldReturn0x20400000000000) {
        U64 expected = 0x20400000000000ULL;
        U64 actual = getKnightBitMask(63);
        ASSERT_EQ(expected, actual);
    }
}
