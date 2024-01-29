#include <gtest/gtest.h>
#include "ChessEngine/bits/PawnBitMasks.h"

namespace bits {
    class PawnBitMasks : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(PawnBitMasks, getPawnBitMask_8asWhite_ShouldReturn0x0000000001030000) {
        U64 expected = 0x0000000001030000ULL;
        U64 actual = getPawnBitMask(8, true);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_12asWhite_ShouldReturn0x0000000010380000) {
        U64 expected = 0x0000000010380000ULL;
        U64 actual = getPawnBitMask(12, true);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_15asWhite_ShouldReturn0x0000000080C00000) {
        U64 expected = 0x0000000080C00000ULL;
        U64 actual = getPawnBitMask(15, true);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_20asWhite_ShouldReturn0x000000038000000) {
        U64 expected = 0x000000038000000ULL;
        U64 actual = getPawnBitMask(20, true);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_48asBlack_ShouldReturn0x000030100000000) {
        U64 expected = 0x000030100000000ULL;
        U64 actual = getPawnBitMask(48, false);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_52asBlack_ShouldReturn0x0000381000000000) {
        U64 expected = 0x0000381000000000ULL;
        U64 actual = getPawnBitMask(52, false);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_55asBlack_ShouldReturn0x0000C08000000000) {
        U64 expected = 0x0000C08000000000ULL;
        U64 actual = getPawnBitMask(55, false);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(PawnBitMasks, getPawnBitMask_44asBlack_ShouldReturn0x0000003800000000) {
        U64 expected = 0x0000003800000000ULL;
        U64 actual = getPawnBitMask(44, false);
        ASSERT_EQ(expected, actual);
    }
}
