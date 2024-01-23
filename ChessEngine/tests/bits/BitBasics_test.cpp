#include <gtest/gtest.h>
#include "ChessEngine/bits/BitBasics.h"

namespace bits {
    class BitBasics : public ::testing::Test {
    protected:
        bits::U64 a;

        void SetUp() override {
            a = 0x000000000000FF00ULL;
        }
    };

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
    TEST_F(BitBasics, fileFromBitIndex_Pos0_ShouldReturn0) {
        int expected = 0;
        int actual = bits::fileFromBitIndex(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos7_ShouldReturn7) {
        int expected = 7;
        int actual = bits::fileFromBitIndex(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos8_ShouldReturn0) {
        int expected = 0;
        int actual = bits::fileFromBitIndex(8);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos15_ShouldReturn7) {
        int expected = 7;
        int actual = bits::fileFromBitIndex(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, fileFromBitIndex_Pos63_ShouldReturn7) {
        int expected = 7;
        int actual = bits::fileFromBitIndex(63);
        ASSERT_EQ(expected, actual);
    }

    // indexOfLSB tests
    TEST_F(BitBasics, indexOfLSB_A_ShouldReturn8) {
        int expected = 8;
        int actual = bits::indexOfLSB(a);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, indexOfLSB_0_ShouldReturn0) {
        int expected = 0;
        int actual = bits::indexOfLSB(0ULL);
        ASSERT_EQ(expected, actual);
    }

    // indexOfMSB tests
    TEST_F(BitBasics, indexOfMSB_A_ShouldReturn15) {
        int expected = 15;
        int actual = bits::indexOfMSB(a);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBasics, indexOfMSB_0_ShouldReturn0) {
        int expected = 0;
        int actual = bits::indexOfMSB(0ULL);
        ASSERT_EQ(expected, actual);
    }
}
