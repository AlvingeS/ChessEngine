#include <gtest/gtest.h>
#include "ChessEngine/game/ChessBoard.h"

namespace game {

    class ChessBoardTest : public ::testing::Test {
        protected:
            ChessBoard chessboard;
            std::string fenOne;

            void SetUp() override {
                chessboard = ChessBoard();
                fenOne = "8/8/8/8/3Pp3/8/8/8";
            }
    };

    TEST_F(ChessBoardTest, ConstructorInit_WhitePawn) {
        bits::U64 expected = 0x000000000000FF00ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteKnight) {
        bits::U64 expected = 0x0000000000000042ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteBishop) {
        bits::U64 expected = 0x0000000000000024ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteRook) {
        bits::U64 expected = 0x0000000000000081ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteQueen) {
        bits::U64 expected = 0x0000000000000010ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteKing) {
        bits::U64 expected = 0x0000000000000008ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackPawn) {
        bits::U64 expected = 0x00FF000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackKnight) {
        bits::U64 expected = 0x4200000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackBishop) {
        bits::U64 expected = 0x2400000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackRook) {
        bits::U64 expected = 0x8100000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackQueen) {
        bits::U64 expected = 0x1000000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackKing) {
        bits::U64 expected = 0x0800000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, getWhitePiecesBitmask_ShouldReturn0x000000000000FFFF) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = chessboard.getWhitePiecesBitmask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, getBlackPiecesBitmask_ShouldReturn0xFFFF000000000000) {
        bits::U64 expected = 0xFFFF000000000000ULL;
        bits::U64 actual = chessboard.getBlackPiecesBitmask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, fillSquaresLookupTest_ShouldReturnStartingPos) {
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(0), PieceType::W_ROOK);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(1), PieceType::W_KNIGHT);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(2), PieceType::W_BISHOP);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(3), PieceType::W_KING);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(4), PieceType::W_QUEEN);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(5), PieceType::W_BISHOP);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(6), PieceType::W_KNIGHT);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(7), PieceType::W_ROOK);

        for (int i = 8; i < 16; i++) {
            ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::W_PAWN);
        }

        for (int i = 16; i < 48; i++) {
            ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::EMPTY);
        }

        for (int i = 48; i < 56; i++) {
            ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::B_PAWN);
        }

        ASSERT_EQ(chessboard.getPieceTypeAtIndex(56), PieceType::B_ROOK);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(57), PieceType::B_KNIGHT);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(58), PieceType::B_BISHOP);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(59), PieceType::B_KING);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(60), PieceType::B_QUEEN);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(61), PieceType::B_BISHOP);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(62), PieceType::B_KNIGHT);
        ASSERT_EQ(chessboard.getPieceTypeAtIndex(63), PieceType::B_ROOK);
    }

    TEST_F(ChessBoardTest, fillSquaresLookupFenOneTest_ShouldReturnTwoPieces) {
        chessboard.setBoardFromFen(fenOne);
    
        for (int i = 0; i < 64; i++) {
            if (i == 27) {
                ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::B_PAWN);
            } else if (i == 28) {
                ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::W_PAWN);
            } else {
                ASSERT_EQ(chessboard.getPieceTypeAtIndex(i), PieceType::EMPTY);
            }
        }
    }
}
