#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "8/2P5/8/8/8/b1R5/4q3/8";
                fenTwo = "7r/8/8/1P6/8/1R1p3R/8/8";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorTest, genRookMoves_fenOneWhite_ShouldReturn12Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 16, 17, 18, 19, 20, 5, 13, 23, 22}, PieceType::W_ROOK); 

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorTest, genRookMoves_fenOneBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorTest, genRookMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorTest, genRookMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorTest, genRookMoves_fenTwoWhite_ShouldReturn15Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 16, {24, 32, 40, 48, 56, 8, 0, 17, 18, 19, 20}, PieceType::W_ROOK);
        insertExpectedMoves(expectedMoves, 22, {30, 21, 20, 14, 6, 23}, PieceType::W_ROOK);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());        
    }

    TEST_F(MoveGeneratorTest, genRookMoves_fenTwoBlack_ShouldReturn12Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 56, {48, 40, 32, 24, 16, 57, 58, 59, 60, 61, 62, 63}, PieceType::B_ROOK);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());   
    }
}
