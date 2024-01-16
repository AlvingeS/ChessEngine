#include <gtest/gtest.h>
#include "ChessEngine/game/chessboard.h"

namespace game {

    class BitboardInit : public ::testing::Test {
        protected:
            game::ChessBoard chessboard;

            void SetUp() override {
                chessboard = game::ChessBoard();
            }
    };

    TEST_F(BitboardInit, WhitePawn) {
        utils::U64 expected = 0x000000000000FF00ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_PAWN);

        ASSERT_EQ(expected, actual) << "WhitePawn: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteKnight) {
        utils::U64 expected = 0x0000000000000042ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_KNIGHT);

        ASSERT_EQ(expected, actual) << "WhiteKnight: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteBishop) {
        utils::U64 expected = 0x0000000000000024ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_BISHOP);

        ASSERT_EQ(expected, actual) << "WhiteBishop: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteRook) {
        utils::U64 expected = 0x0000000000000081ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_ROOK);

        ASSERT_EQ(expected, actual) << "WhiteRook: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteQueen) {
        utils::U64 expected = 0x0000000000000010ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_QUEEN);

        ASSERT_EQ(expected, actual) << "WhiteQueen: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteKing) {
        utils::U64 expected = 0x0000000000000008ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_KING);

        ASSERT_EQ(expected, actual) << "WhiteKing: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackPawn) {
        utils::U64 expected = 0x00FF000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_PAWN);

        ASSERT_EQ(expected, actual) << "BlackPawn: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackKnight) {
        utils::U64 expected = 0x4200000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_KNIGHT);

        ASSERT_EQ(expected, actual) << "BlackKnight: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackBishop) {
        utils::U64 expected = 0x2400000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_BISHOP);

        ASSERT_EQ(expected, actual) << "BlackBishop: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackRook) {
        utils::U64 expected = 0x8100000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_ROOK);

        ASSERT_EQ(expected, actual) << "BlackRook: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackQueen) {
        utils::U64 expected = 0x1000000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_QUEEN);

        ASSERT_EQ(expected, actual) << "BlackQueen: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackKing) {
        utils::U64 expected = 0x0800000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_KING);

        ASSERT_EQ(expected, actual) << "BlackKing: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, WhiteAll) {
        utils::U64 expected = 0x000000000000FFFFULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::W_ALL);

        ASSERT_EQ(expected, actual) << "WhiteAll: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, BlackAll) {
        utils::U64 expected = 0xFFFF000000000000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::B_ALL);

        ASSERT_EQ(expected, actual) << "BlackAll: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, Occupied) {
        utils::U64 expected = 0xFFFF00000000FFFFULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::OCCUPIED);

        ASSERT_EQ(expected, actual) << "Occupied: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitboardInit, Empty) {
        utils::U64 expected = 0x0000FFFFFFFF0000ULL;
        utils::U64 actual = chessboard.get_bitboard(PieceType::EMPTY);

        ASSERT_EQ(expected, actual) << "Empty: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }
}
