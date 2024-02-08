#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorBishopTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "5P2/4b3/1P6/2B2B2/1r3b2/8/8/8";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneWhite_ShouldReturn18Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genBishopMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 34, {41, 48, 25, 16, 27, 20, 13, 6, 43, 52, 61}, PieceType::W_BISHOP);
        insertExpectedMoves(expectedMoves, 37, {44, 51, 28, 19, 10, 1, 30}, PieceType::W_BISHOP);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneBlack_ShouldReturn18Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genBishopMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 51, {58, 42, 33, 24, 44, 37, 60}, PieceType::B_BISHOP);
        insertExpectedMoves(expectedMoves, 26, {33, 40, 17, 8, 19, 12, 5, 35, 44, 53, 62}, PieceType::B_BISHOP);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genBishopMoves(true);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genBishopMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }
}
