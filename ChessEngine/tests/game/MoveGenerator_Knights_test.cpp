#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorKnightTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "8/1n1n4/5R2/2N5/4P3/8/5p2/7N";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosWhite_ShouldReturn4Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKnightMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 1, {16, 18}, PieceType::W_KNIGHT);
        insertExpectedMoves(expectedMoves, 6, {21, 23}, PieceType::W_KNIGHT);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosBlack_ShouldReturn4Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKnightMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 57, {40, 42}, PieceType::B_KNIGHT);
        insertExpectedMoves(expectedMoves, 62, {45, 47}, PieceType::B_KNIGHT);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneWhite_ShouldReturn8Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKnightMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 0, {10, 17}, PieceType::W_KNIGHT);
        insertExpectedMoves(expectedMoves, 37, {52, 43, 20, 22, 31, 47, 54}, PieceType::W_KNIGHT);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneBlack_ShouldReturn10Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKnightMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 52, {58, 42, 35, 37, 46, 62}, PieceType::B_KNIGHT);
        insertExpectedMoves(expectedMoves, 54, {60, 44, 37, 39}, PieceType::B_KNIGHT);

        for (Move move : moves) {
            auto found = expectedMoves.find(move);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }
}
