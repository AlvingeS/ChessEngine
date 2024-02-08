#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorKingTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "5r2/3qk3/4R3/8/8/8/Pn6/1K6";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneWhite_ShouldReturn4Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 6, {14, 13, 5, 7}, PieceType::W_KING);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneBlack_ShouldReturn6Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKingMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 51, {59, 50, 42, 43, 44, 60}, PieceType::B_KING);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKingMoves(true);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKingMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }
}
