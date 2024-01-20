#include <gtest/gtest.h>
#include "ChessEngine/bits/BitMasks.h"

namespace bits {
    class BitMasks : public ::testing::Test {
        protected:
            void SetUp() override {}
    };

    // bitwiseAnd tests
    TEST_F(BitMasks, getFileMask_File0_ShouldReturn0x8080808080808080) {
        U64 expected = 0x8080808080808080ULL;
        U64 actual = getFileMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File1_ShouldReturn0x4040404040404040) {
        U64 expected = 0x4040404040404040ULL;
        U64 actual = getFileMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File2_ShouldReturn0x2020202020202020) {
        U64 expected = 0x2020202020202020ULL;
        U64 actual = getFileMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File3_ShouldReturn0x1010101010101010) {
        U64 expected = 0x1010101010101010ULL;
        U64 actual = getFileMask(3);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File4_ShouldReturn0x0808080808080808) {
        U64 expected = 0x0808080808080808ULL;
        U64 actual = getFileMask(4);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File5_ShouldReturn0x0404040404040404) {
        U64 expected = 0x0404040404040404ULL;
        U64 actual = getFileMask(5);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File6_ShouldReturn0x0202020202020202) {
        U64 expected = 0x0202020202020202ULL;
        U64 actual = getFileMask(6);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getFileMask_File7_ShouldReturn0x0101010101010101) {
        U64 expected = 0x0101010101010101ULL;
        U64 actual = getFileMask(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank0_ShouldReturn0x00000000000000FF) {
        U64 expected = 0x00000000000000FFULL;
        U64 actual = getRankMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank1_ShouldReturn0x000000000000FF00) {
        U64 expected = 0x000000000000FF00ULL;
        U64 actual = getRankMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank2_ShouldReturn0x0000000000FF0000) {
        U64 expected = 0x0000000000FF0000ULL;
        U64 actual = getRankMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank3_ShouldReturn0x00000000FF000000) {
        U64 expected = 0x00000000FF000000ULL;
        U64 actual = getRankMask(3);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank4_ShouldReturn0x000000FF00000000) {
        U64 expected = 0x000000FF00000000ULL;
        U64 actual = getRankMask(4);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank5_ShouldReturn0x0000FF0000000000) {
        U64 expected = 0x0000FF0000000000ULL;
        U64 actual = getRankMask(5);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank6_ShouldReturn0x00FF000000000000) {
        U64 expected = 0x00FF000000000000ULL;
        U64 actual = getRankMask(6);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitMasks, getRankMask_Rank7_ShouldReturn0xFF00000000000000) {
        U64 expected = 0xFF00000000000000ULL;
        U64 actual = getRankMask(7);
        ASSERT_EQ(expected, actual);
    }
}
