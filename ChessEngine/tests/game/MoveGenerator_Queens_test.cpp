#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorQueenTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "5k2/2P5/5q2/b4N2/1q6/2Q2Q2/8/8";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneBlack_ShouldReturnXMoves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genQueenMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 42, {50, 49, 56, 41, 40, 33, 24, 34, 35, 28, 21, 43, 44, 45, 46, 47, 51, 60, 50}, PieceType::B_QUEEN);
        insertExpectedMoves(expectedMoves, 30, {38, 46, 54, 62, 37, 44, 51, 29, 28, 27, 26, 25, 24, 21, 22, 14, 6, 23, 31}, PieceType::B_QUEEN);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneWhite_ShouldReturnXMoves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genQueenMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 28, 35, 42, 20, 19, 12, 3, 13, 5, 14, 7, 22, 23, 30}, PieceType::W_QUEEN);
        insertExpectedMoves(expectedMoves, 18, {26, 25, 32, 17, 16, 9, 0, 10, 2, 11, 4, 19, 20, 27, 36, 45, 54, 63}, PieceType::W_QUEEN);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genQueenMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genQueenMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }
}
