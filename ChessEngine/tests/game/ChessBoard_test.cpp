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
        U64 expected = 0x000000000000FF00ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteKnight) {
        U64 expected = 0x0000000000000042ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteBishop) {
        U64 expected = 0x0000000000000024ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteRook) {
        U64 expected = 0x0000000000000081ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteQueen) {
        U64 expected = 0x0000000000000010ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_WhiteKing) {
        U64 expected = 0x0000000000000008ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::W_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackPawn) {
        U64 expected = 0x00FF000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackKnight) {
        U64 expected = 0x4200000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackBishop) {
        U64 expected = 0x2400000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackRook) {
        U64 expected = 0x8100000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackQueen) {
        U64 expected = 0x1000000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, ConstructorInit_BlackKing) {
        U64 expected = 0x0800000000000000ULL;
        U64 actual = chessboard.getBitboardFromPieceType(PieceType::B_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, getWhitePiecesBitmask_ShouldReturn0x000000000000FFFF) {
        U64 expected = 0x000000000000FFFFULL;
        U64 actual = chessboard.getWhitePiecesBitmask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, getBlackPiecesBitmask_ShouldReturn0xFFFF000000000000) {
        U64 expected = 0xFFFF000000000000ULL;
        U64 actual = chessboard.getBlackPiecesBitmask();

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
