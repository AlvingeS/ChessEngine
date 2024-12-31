#include <gtest/gtest.h>

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"
#include "ChessEngine/src/model/board/SquaresLookup.h"

#include "ChessEngine/src/io/Fen.h"

namespace model {

class BitboardsTest : public ::testing::Test 
{
protected:
    Bitboards bitboards;
    GameStateBitmasks gameStateBitmasks;
    SquaresLookup squaresLookup;

    std::string fenOne;

    BitboardsTest()
        : bitboards(), 
        gameStateBitmasks(bitboards), 
        squaresLookup(bitboards), 
        fenOne("8/8/8/8/3Pp3/8/8/8") 
    {
        // Constructor body if needed
    }

    void SetUp() override 
    {
        // Additional setup if needed, but member variables are already initialized
    }
};


TEST_F(BitboardsTest, ConstructorInit_WhitePawn) 
{
    bitboard expected = 0x000000000000FF00ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKnight) 
{
    bitboard expected = 0x0000000000000042ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteBishop) 
{
    bitboard expected = 0x0000000000000024ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteRook) 
{
    bitboard expected = 0x0000000000000081ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteQueen) 
{
    bitboard expected = 0x0000000000000010ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKing) 
{
    bitboard expected = 0x0000000000000008ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::W_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackPawn) 
{
    bitboard expected = 0x00FF000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKnight) 
{
    bitboard expected = 0x4200000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackBishop) 
{
    bitboard expected = 0x2400000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackRook) 
{
    bitboard expected = 0x8100000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackQueen) 
{
    bitboard expected = 0x1000000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKing) 
{
    bitboard expected = 0x0800000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(PieceType::B_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getWhitePiecesbitboard_ShouldReturn0x000000000000FFFF) 
{
    bitboard expected = 0x000000000000FFFFULL;
    bitboard actual = gameStateBitmasks.getWhitePiecesBitmask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getBlackPiecesbitboard_ShouldReturn0xFFFF000000000000) 
{
    bitboard expected = 0xFFFF000000000000ULL;
    bitboard actual = gameStateBitmasks.getBlackPiecesBitmask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, fillSquaresLookupTest_ShouldReturnStartingPos) 
{
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(0), PieceType::W_ROOK);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(1), PieceType::W_KNIGHT);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(2), PieceType::W_BISHOP);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(3), PieceType::W_KING);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(4), PieceType::W_QUEEN);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(5), PieceType::W_BISHOP);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(6), PieceType::W_KNIGHT);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(7), PieceType::W_ROOK);

    for (int i = 8; i < 16; i++) {
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::W_PAWN);
    }

    for (int i = 16; i < 48; i++) {
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::EMPTY);
    }

    for (int i = 48; i < 56; i++) {
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::B_PAWN);
    }

    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(56), PieceType::B_ROOK);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(57), PieceType::B_KNIGHT);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(58), PieceType::B_BISHOP);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(59), PieceType::B_KING);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(60), PieceType::B_QUEEN);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(61), PieceType::B_BISHOP);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(62), PieceType::B_KNIGHT);
    ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(63), PieceType::B_ROOK);
}

TEST_F(BitboardsTest, fillSquaresLookupFenOneTest_ShouldReturnTwoPieces) 
{
    io::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);

    for (int i = 0; i < 64; i++) {
        if (i == 27) {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::B_PAWN);
        } else if (i == 28) {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::W_PAWN);
        } else {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), PieceType::EMPTY);
        }
    }
}

} // namespace model