#include <gtest/gtest.h>
#include "ChessEngine/game/ChessBoard.h"

namespace game {

    class BitboardInit : public ::testing::Test {
        protected:
            game::ChessBoard chessboard;

            void SetUp() override {
                chessboard = game::ChessBoard();
            }
    };

    TEST_F(BitboardInit, WhitePawn) {
        bits::U64 expected = 0x000000000000FF00ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_PAWN);

        ASSERT_EQ(expected, actual) << "WhitePawn: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteKnight) {
        bits::U64 expected = 0x0000000000000042ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_KNIGHT);

        ASSERT_EQ(expected, actual) << "WhiteKnight: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteBishop) {
        bits::U64 expected = 0x0000000000000024ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_BISHOP);

        ASSERT_EQ(expected, actual) << "WhiteBishop: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteRook) {
        bits::U64 expected = 0x0000000000000081ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_ROOK);

        ASSERT_EQ(expected, actual) << "WhiteRook: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteQueen) {
        bits::U64 expected = 0x0000000000000010ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_QUEEN);

        ASSERT_EQ(expected, actual) << "WhiteQueen: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteKing) {
        bits::U64 expected = 0x0000000000000008ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::W_KING);

        ASSERT_EQ(expected, actual) << "WhiteKing: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackPawn) {
        bits::U64 expected = 0x00FF000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_PAWN);

        ASSERT_EQ(expected, actual) << "BlackPawn: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackKnight) {
        bits::U64 expected = 0x4200000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_KNIGHT);

        ASSERT_EQ(expected, actual) << "BlackKnight: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackBishop) {
        bits::U64 expected = 0x2400000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_BISHOP);

        ASSERT_EQ(expected, actual) << "BlackBishop: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackRook) {
        bits::U64 expected = 0x8100000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_ROOK);

        ASSERT_EQ(expected, actual) << "BlackRook: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackQueen) {
        bits::U64 expected = 0x1000000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_QUEEN);

        ASSERT_EQ(expected, actual) << "BlackQueen: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackKing) {
        bits::U64 expected = 0x0800000000000000ULL;
        bits::U64 actual = chessboard.getBitboard(PieceType::B_KING);

        ASSERT_EQ(expected, actual) << "BlackKing: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }
}
