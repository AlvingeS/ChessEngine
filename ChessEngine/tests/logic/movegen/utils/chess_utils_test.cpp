#include <gtest/gtest.h>

#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

class ChessUtilsTest : public ::testing::Test 
{
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

TEST_F(ChessUtilsTest, getFileMask_File7_ShouldReturn0x8080808080808080)
{
    bitmask expected = 0x8080808080808080ULL;
    bitmask actual = ChessUtils::getFileMask(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File6_ShouldReturn0x4040404040404040)
{
    bitmask expected = 0x4040404040404040ULL;
    bitmask actual = ChessUtils::getFileMask(6);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File5_ShouldReturn0x2020202020202020)
{
    bitmask expected = 0x2020202020202020ULL;
    bitmask actual = ChessUtils::getFileMask(5);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File4_ShouldReturn0x1010101010101010)
{
    bitmask expected = 0x1010101010101010ULL;
    bitmask actual = ChessUtils::getFileMask(4);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File3_ShouldReturn0x0808080808080808)
{
    bitmask expected = 0x0808080808080808ULL;
    bitmask actual = ChessUtils::getFileMask(3);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File2_ShouldReturn0x0404040404040404)
{
    bitmask expected = 0x0404040404040404ULL;
    bitmask actual = ChessUtils::getFileMask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File1_ShouldReturn0x0202020202020202)
{
    bitmask expected = 0x0202020202020202ULL;
    bitmask actual = ChessUtils::getFileMask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File0_ShouldReturn0x0101010101010101)
{
    bitmask expected = 0x0101010101010101ULL;
    bitmask actual = ChessUtils::getFileMask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank0_ShouldReturn0x00000000000000FF)
{
    bitmask expected = 0x00000000000000FFULL;
    bitmask actual = ChessUtils::getRankMask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank1_ShouldReturn0x000000000000FF00)
{
    bitmask expected = 0x000000000000FF00ULL;
    bitmask actual = ChessUtils::getRankMask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank2_ShouldReturn0x0000000000FF0000)
{
    bitmask expected = 0x0000000000FF0000ULL;
    bitmask actual = ChessUtils::getRankMask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank3_ShouldReturn0x00000000FF000000)
{
    bitmask expected = 0x00000000FF000000ULL;
    bitmask actual = ChessUtils::getRankMask(3);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank4_ShouldReturn0x000000FF00000000)
{
    bitmask expected = 0x000000FF00000000ULL;
    bitmask actual = ChessUtils::getRankMask(4);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank5_ShouldReturn0x0000FF0000000000)
{
    bitmask expected = 0x0000FF0000000000ULL;
    bitmask actual = ChessUtils::getRankMask(5);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank6_ShouldReturn0x00FF000000000000)
{
    bitmask expected = 0x00FF000000000000ULL;
    bitmask actual = ChessUtils::getRankMask(6);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank7_ShouldReturn0xFF00000000000000)
{
    bitmask expected = 0xFF00000000000000ULL;
    bitmask actual = ChessUtils::getRankMask(7);
    ASSERT_EQ(expected, actual);
}

    // ChessUtils::rankFromBitIndex tests
TEST_F(ChessUtilsTest, rankFromBitIndex_Pos0_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::rankFromBitIndex(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos7_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::rankFromBitIndex(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos8_ShouldReturn1)
{
    int expected = 1;
    int actual = ChessUtils::rankFromBitIndex(8);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos15_ShouldReturn1)
{
    int expected = 1;
    int actual = ChessUtils::rankFromBitIndex(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos63_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::rankFromBitIndex(63);
    ASSERT_EQ(expected, actual);
}

// ChessUtils::fileFromBitIndex tests
TEST_F(ChessUtilsTest, fileFromBitIndex_Pos0_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::fileFromBitIndex(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos7_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos8_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::fileFromBitIndex(8);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos15_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos63_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(63);
    ASSERT_EQ(expected, actual);
}


TEST_F(ChessUtilsTest, getBitIndices_a_ShouldReturn0_8_10_16to23)
{
    std::vector<int> expected = {0, 8, 10, 16, 17, 18, 19, 20, 21, 22, 23};
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, a);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getBitIndices_b_ShouldReturnEmptyVector)
{
    std::vector<int> expected = {};
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, b);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getBitIndices_c_ShouldReturn0to63)
{
    std::vector<int> expected = {};

    for (int i = 0; i < 64; i++)
{
        expected.push_back(i);
    }
    
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, c);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getOccupiedSquaresBitmask_StartingPosition_ShouldReturn0xFFFF00000000FFFF)
{
    bitmask expected = 0xFFFF00000000FFFFULL;
    bitmask actual = ChessUtils::getOccupiedSquaresBitmask(whitePieces, blackPieces);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getEmptySquaresBitmask_StartingPosition_ShouldReturn0x0000FFFFFFFF0000)
{
    bitmask expected = 0x0000FFFFFFFF0000ULL;
    bitmask actual = ChessUtils::getEmptySquaresBitmask(whitePieces, blackPieces);
    ASSERT_EQ(expected, actual);
}

} // namespace logic