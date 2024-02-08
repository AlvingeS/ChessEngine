#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorPawnTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenEnPessantForWhiteTest;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "8/5p1p/5R1p/2p5/3P4/p7/5P1P/8";
                fenEnPessantForWhiteTest = "rnbqkb1r/pppp1ppp/5n2/3Pp3/8/8/8/RNBQKBNR";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosWhite_ShouldReturn16Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genPawnMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 8, {16, 24}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 9, {17, 25}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 10, {18, 26}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 11, {19, 27}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 12, {20, 28}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 13, {21, 29}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 14, {22, 30}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 15, {23, 31}, PieceType::W_PAWN);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosBlack_ShouldReturn16Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genPawnMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 48, {40, 32}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 49, {41, 33}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 50, {42, 34}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 51, {43, 35}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 52, {44, 36}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 53, {45, 37}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 54, {46, 38}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 55, {47, 39}, PieceType::B_PAWN);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneWhite_ShouldReturn6Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genPawnMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 28, {37, 36}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 10, {18, 26}, PieceType::W_PAWN);
        insertExpectedMoves(expectedMoves, 8, {16, 24}, PieceType::W_PAWN);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneBlack_ShouldReturnXMoves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genPawnMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 40, {32}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 37, {28, 29}, PieceType::B_PAWN);
        insertExpectedMoves(expectedMoves, 23, {15}, PieceType::B_PAWN);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForWhite_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenEnPessantForWhiteTest);
        moveGenerator.genPawnMoves(true);
        moveGenerator.getBoard().setEnPessantTargetAtIndex(43);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 36, {44, 43}, PieceType::W_PAWN);

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }
}
