#include <gtest/gtest.h>
#include "ChessEngine/bits/BitBasics.h"

namespace bits {
    class BitBasics : public ::testing::Test {
    protected:
        bits::U64 a;
        bits::U64 b;
        bits::U64 c;
        bits::U64 d;

        void SetUp() override {
            a = 0x000000000000FF00ULL;
            b = 0x00000000000000FFULL;
            c = 0x00000000000000F0ULL;
            d = 0x0000000000000000ULL;
        }
    };

    // bitwiseAnd tests
    TEST_F(BitBasics, bitwiseAnd_AandB_ShouldReturnZero) {
        bits::U64 expected = 0x0000000000000000ULL;
        bits::U64 actual = bits::bitwiseAnd(a, b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseAnd_BandC_ShouldReturnF0) {
        bits::U64 expected = 0x00000000000000F0ULL;
        bits::U64 actual = bits::bitwiseAnd(b, c);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseAnd_CandD_ShouldReturnZero) {
        bits::U64 expected = 0x0000000000000000ULL;
        bits::U64 actual = bits::bitwiseAnd(c, d);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseAnd_AandD_ShouldReturnZero) {
        bits::U64 expected = 0x0000000000000000ULL;
        bits::U64 actual = bits::bitwiseAnd(a, d);
        ASSERT_EQ(expected, actual);
    }

    // bitwiseOr tests
    TEST_F(BitBasics, bitwiseOr_AandB_ShouldReturnFFFF) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = bits::bitwiseOr(a, b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseOr_BandC_ShouldReturnFF) {
        bits::U64 expected = 0x00000000000000FFULL;
        bits::U64 actual = bits::bitwiseOr(b, c);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseOr_CandD_ShouldReturnF0) {
        bits::U64 expected = 0x00000000000000F0ULL;
        bits::U64 actual = bits::bitwiseOr(c, d);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseOr_AandD_ShouldReturnFF00) {
        bits::U64 expected = 0x000000000000FF00ULL;
        bits::U64 actual = bits::bitwiseOr(a, d);
        ASSERT_EQ(expected, actual);
    }

    // bitwiseXor tests
    TEST_F(BitBasics, bitwiseXor_AandB_ShouldReturnFFFF) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = bits::bitwiseXor(a, b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseXor_BandC_ShouldReturn0F) {
        bits::U64 expected = 0x000000000000000FULL;
        bits::U64 actual = bits::bitwiseXor(b, c);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseXor_CandD_ShouldReturnF0) {
        bits::U64 expected = 0x00000000000000F0ULL;
        bits::U64 actual = bits::bitwiseXor(c, d);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseXor_AandD_ShouldReturnFF00) {
        bits::U64 expected = 0x000000000000FF00ULL;
        bits::U64 actual = bits::bitwiseXor(a, d);
        ASSERT_EQ(expected, actual);
    }

    // bitwiseNot tests
    TEST_F(BitBasics, bitwiseNot_A_ShouldReturnFFFF00FF) {
        bits::U64 expected = 0xFFFFFFFFFFFF00FFULL;
        bits::U64 actual = bits::bitwiseNot(a);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseNot_B_ShouldReturnFFFFFF00) {
        bits::U64 expected = 0xFFFFFFFFFFFFFF00ULL;
        bits::U64 actual = bits::bitwiseNot(b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseNot_C_ShouldReturnFFFFFF0F) {
        bits::U64 expected = 0xFFFFFFFFFFFFFF0FULL;
        bits::U64 actual = bits::bitwiseNot(c);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, bitwiseNot_D_ShouldReturnFFFFFFFF) {
        bits::U64 expected = 0xFFFFFFFFFFFFFFFFULL;
        bits::U64 actual = bits::bitwiseNot(d);
        ASSERT_EQ(expected, actual);
    }

    // getBit tests
    TEST_F(BitBasics, getBit_A_Pos0_ShouldReturnFalse) {
        bool expected = 0;
        bool actual = bits::getBit(a, 0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, getBit_A_Pos9_ShouldReturnTrue) {
        bool expected = 1;
        bool actual = bits::getBit(a, 9);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, getBit_A_Pos15_ShouldReturnTrue) {
        bool expected = 1;
        bool actual = bits::getBit(a, 15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, getBit_A_Pos16_ShouldReturnFalse) {
        bool expected = 0;
        bool actual = bits::getBit(a, 16);
        ASSERT_EQ(expected, actual);
    }

    // rankFromBitIndex tests
    TEST_F(BitBasics, rankFromBitIndex_Pos0_ShouldReturn0) {
        int expected = 0;
        int actual = bits::rankFromBitIndex(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, rankFromBitIndex_Pos7_ShouldReturn0) {
        int expected = 0;
        int actual = bits::rankFromBitIndex(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, rankFromBitIndex_Pos8_ShouldReturn1) {
        int expected = 1;
        int actual = bits::rankFromBitIndex(8);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, rankFromBitIndex_Pos15_ShouldReturn1) {
        int expected = 1;
        int actual = bits::rankFromBitIndex(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, rankFromBitIndex_Pos63_ShouldReturn7) {
        int expected = 7;
        int actual = bits::rankFromBitIndex(63);
        ASSERT_EQ(expected, actual);
    }

    // fileFromBitIndex tests
    TEST_F(BitBasics, fileFromBitIndex_Pos0_ShouldReturn7) {
        int expected = 7;
        int actual = bits::fileFromBitIndex(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos7_ShouldReturn0) {
        int expected = 0;
        int actual = bits::fileFromBitIndex(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos8_ShouldReturn7) {
        int expected = 7;
        int actual = bits::fileFromBitIndex(8);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos15_ShouldReturn0) {
        int expected = 0;
        int actual = bits::fileFromBitIndex(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos63_ShouldReturn0) {
        int expected = 0;
        int actual = bits::fileFromBitIndex(63);
        ASSERT_EQ(expected, actual);
    }
}
