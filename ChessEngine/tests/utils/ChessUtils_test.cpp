#include <gtest/gtest.h>

#include "ChessEngine/src/utils/ChessUtils.h"
#include "ChessEngine/src/utils/BitBasics.h"

namespace utils {
    class ChessUtils : public ::testing::Test {
        protected:
            bitmask a;
            bitmask b;
            bitmask c;
            bitmask whitePieces;
            bitmask blackPieces;
            void SetUp() override {
                a = 0x0000000000FF0501;
                b = 0x0000000000000000;
                c = 0xFFFFFFFFFFFFFFFF;
                whitePieces = 0x000000000000FFFFULL;
                blackPieces = 0xFFFF000000000000ULL;
            }
    };

    TEST_F(ChessUtils, getFileMask_File7_ShouldReturn0x8080808080808080) {
        bitmask expected = 0x8080808080808080ULL;
        bitmask actual = getFileMask(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File6_ShouldReturn0x4040404040404040) {
        bitmask expected = 0x4040404040404040ULL;
        bitmask actual = getFileMask(6);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File5_ShouldReturn0x2020202020202020) {
        bitmask expected = 0x2020202020202020ULL;
        bitmask actual = getFileMask(5);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File4_ShouldReturn0x1010101010101010) {
        bitmask expected = 0x1010101010101010ULL;
        bitmask actual = getFileMask(4);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File3_ShouldReturn0x0808080808080808) {
        bitmask expected = 0x0808080808080808ULL;
        bitmask actual = getFileMask(3);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File2_ShouldReturn0x0404040404040404) {
        bitmask expected = 0x0404040404040404ULL;
        bitmask actual = getFileMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File1_ShouldReturn0x0202020202020202) {
        bitmask expected = 0x0202020202020202ULL;
        bitmask actual = getFileMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getFileMask_File0_ShouldReturn0x0101010101010101) {
        bitmask expected = 0x0101010101010101ULL;
        bitmask actual = getFileMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank0_ShouldReturn0x00000000000000FF) {
        bitmask expected = 0x00000000000000FFULL;
        bitmask actual = getRankMask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank1_ShouldReturn0x000000000000FF00) {
        bitmask expected = 0x000000000000FF00ULL;
        bitmask actual = getRankMask(1);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank2_ShouldReturn0x0000000000FF0000) {
        bitmask expected = 0x0000000000FF0000ULL;
        bitmask actual = getRankMask(2);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank3_ShouldReturn0x00000000FF000000) {
        bitmask expected = 0x00000000FF000000ULL;
        bitmask actual = getRankMask(3);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank4_ShouldReturn0x000000FF00000000) {
        bitmask expected = 0x000000FF00000000ULL;
        bitmask actual = getRankMask(4);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank5_ShouldReturn0x0000FF0000000000) {
        bitmask expected = 0x0000FF0000000000ULL;
        bitmask actual = getRankMask(5);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank6_ShouldReturn0x00FF000000000000) {
        bitmask expected = 0x00FF000000000000ULL;
        bitmask actual = getRankMask(6);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getRankMask_Rank7_ShouldReturn0xFF00000000000000) {
        bitmask expected = 0xFF00000000000000ULL;
        bitmask actual = getRankMask(7);
        ASSERT_EQ(expected, actual);
    }

        // rankFromBitIndex tests
    TEST_F(ChessUtils, rankFromBitIndex_Pos0_ShouldReturn0) {
        int expected = 0;
        int actual = rankFromBitIndex(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, rankFromBitIndex_Pos7_ShouldReturn0) {
        int expected = 0;
        int actual = rankFromBitIndex(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, rankFromBitIndex_Pos8_ShouldReturn1) {
        int expected = 1;
        int actual = rankFromBitIndex(8);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, rankFromBitIndex_Pos15_ShouldReturn1) {
        int expected = 1;
        int actual = rankFromBitIndex(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, rankFromBitIndex_Pos63_ShouldReturn7) {
        int expected = 7;
        int actual = rankFromBitIndex(63);
        ASSERT_EQ(expected, actual);
    }

    // fileFromBitIndex tests
    TEST_F(ChessUtils, fileFromBitIndex_Pos0_ShouldReturn0) {
        int expected = 0;
        int actual = fileFromBitIndex(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, fileFromBitIndex_Pos7_ShouldReturn7) {
        int expected = 7;
        int actual = fileFromBitIndex(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, fileFromBitIndex_Pos8_ShouldReturn0) {
        int expected = 0;
        int actual = fileFromBitIndex(8);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, fileFromBitIndex_Pos15_ShouldReturn7) {
        int expected = 7;
        int actual = fileFromBitIndex(15);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, fileFromBitIndex_Pos63_ShouldReturn7) {
        int expected = 7;
        int actual = fileFromBitIndex(63);
        ASSERT_EQ(expected, actual);
    }

    
    TEST_F(ChessUtils, getBitIndices_a_ShouldReturn0_8_10_16to23) {
        std::vector<int> expected = {0, 8, 10, 16, 17, 18, 19, 20, 21, 22, 23};
        std::vector<int> actual;
        getBitIndices(actual, a);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getBitIndices_b_ShouldReturnEmptyVector) {
        std::vector<int> expected = {};
        std::vector<int> actual;
        getBitIndices(actual, b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getBitIndices_c_ShouldReturn0to63) {
        std::vector<int> expected = {};

        for (int i = 0; i < 64; i++) {
            expected.push_back(i);
        }
        
        std::vector<int> actual;
        getBitIndices(actual, c);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getOccupiedSquaresBitmask_StartingPosition_ShouldReturn0xFFFF00000000FFFF) {
        bitmask expected = 0xFFFF00000000FFFFULL;
        bitmask actual = getOccupiedSquaresBitmask(whitePieces, blackPieces);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessUtils, getEmptySquaresBitmask_StartingPosition_ShouldReturn0x0000FFFFFFFF0000) {
        bitmask expected = 0x0000FFFFFFFF0000ULL;
        bitmask actual = getEmptySquaresBitmask(whitePieces, blackPieces);
        ASSERT_EQ(expected, actual);
    }
}