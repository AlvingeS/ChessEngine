#include <gtest/gtest.h>
#include "ChessEngine/game/ChessBoard.h"

namespace game {

    class ChessBoardTest : public ::testing::Test {
        protected:
            ChessBoard chessboard;

            void SetUp() override {
                chessboard = ChessBoard();
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

    TEST_F(ChessBoardTest, getWhitePiecesBitMask_ShouldReturn0x000000000000FFFF) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = chessboard.getWhitePiecesBitMask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(ChessBoardTest, getBlackPiecesBitMask_ShouldReturn0xFFFF000000000000) {
        bits::U64 expected = 0xFFFF000000000000ULL;
        bits::U64 actual = chessboard.getBlackPiecesBitMask();

        ASSERT_EQ(expected, actual);
    }
}
