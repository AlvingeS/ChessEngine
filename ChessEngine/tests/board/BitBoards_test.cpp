#include <gtest/gtest.h>
#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/utils/Fen.h"

namespace board {

    class BitBoardsTest : public ::testing::Test {
        protected:
            BitBoards bitboards;
            GameStateBitMasks gameStateBitmasks;
            SquaresLookup squaresLookup;

            std::string fenOne;

            BitBoardsTest()
                : bitboards(), 
                gameStateBitmasks(bitboards), 
                squaresLookup(bitboards), 
                fenOne("8/8/8/8/3Pp3/8/8/8") {
                // Constructor body if needed
            }

            void SetUp() override {
                // Additional setup if needed, but member variables are already initialized
            }
    };


    TEST_F(BitBoardsTest, ConstructorInit_WhitePawn) {
        bitmask expected = 0x000000000000FF00ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_WhiteKnight) {
        bitmask expected = 0x0000000000000042ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_WhiteBishop) {
        bitmask expected = 0x0000000000000024ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_WhiteRook) {
        bitmask expected = 0x0000000000000081ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_WhiteQueen) {
        bitmask expected = 0x0000000000000010ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_WhiteKing) {
        bitmask expected = 0x0000000000000008ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::W_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackPawn) {
        bitmask expected = 0x00FF000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_PAWN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackKnight) {
        bitmask expected = 0x4200000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_KNIGHT);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackBishop) {
        bitmask expected = 0x2400000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_BISHOP);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackRook) {
        bitmask expected = 0x8100000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_ROOK);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackQueen) {
        bitmask expected = 0x1000000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_QUEEN);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, ConstructorInit_BlackKing) {
        bitmask expected = 0x0800000000000000ULL;
        bitmask actual = bitboards.getBitboardFromPieceType(PieceType::B_KING);

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, getWhitePiecesBitmask_ShouldReturn0x000000000000FFFF) {
        bitmask expected = 0x000000000000FFFFULL;
        bitmask actual = gameStateBitmasks.getWhitePiecesBitmask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, getBlackPiecesBitmask_ShouldReturn0xFFFF000000000000) {
        bitmask expected = 0xFFFF000000000000ULL;
        bitmask actual = gameStateBitmasks.getBlackPiecesBitmask();

        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitBoardsTest, fillSquaresLookupTest_ShouldReturnStartingPos) {
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(0), board::PieceType::W_ROOK);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(1), board::PieceType::W_KNIGHT);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(2), board::PieceType::W_BISHOP);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(3), board::PieceType::W_KING);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(4), board::PieceType::W_QUEEN);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(5), board::PieceType::W_BISHOP);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(6), board::PieceType::W_KNIGHT);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(7), board::PieceType::W_ROOK);

        for (int i = 8; i < 16; i++) {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::W_PAWN);
        }

        for (int i = 16; i < 48; i++) {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::EMPTY);
        }

        for (int i = 48; i < 56; i++) {
            ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::B_PAWN);
        }

        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(56), board::PieceType::B_ROOK);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(57), board::PieceType::B_KNIGHT);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(58), board::PieceType::B_BISHOP);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(59), board::PieceType::B_KING);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(60), board::PieceType::B_QUEEN);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(61), board::PieceType::B_BISHOP);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(62), board::PieceType::B_KNIGHT);
        ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(63), board::PieceType::B_ROOK);
    }

    TEST_F(BitBoardsTest, fillSquaresLookupFenOneTest_ShouldReturnTwoPieces) {
        utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    
        for (int i = 0; i < 64; i++) {
            if (i == 27) {
                ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::B_PAWN);
            } else if (i == 28) {
                ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::W_PAWN);
            } else {
                ASSERT_EQ(squaresLookup.getPieceTypeAtIndex(i), board::PieceType::EMPTY);
            }
        }
    }
}
