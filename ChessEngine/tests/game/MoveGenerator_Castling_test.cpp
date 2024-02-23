#include <gtest/gtest.h>
#include "BaseMoveGeneratorTest.h"

namespace game {

    class MoveGeneratorCastlingTest : public BaseMoveGeneratorTest {
        protected:
            std::string fenOne;
            std::string fenTwo;
            std::string fenThree;

            void SetUp() override {
                BaseMoveGeneratorTest::SetUp();
                fenOne = "r3k2r/8/8/8/8/8/8/R3K2R";
                fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
                fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";
            }
    };

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhite_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genCastlingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 0, {0, 0}, {Move::KING_CASTLE_FLAG, Move::QUEEN_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlack_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genCastlingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 0, {0, 0}, {Move::KING_CASTLE_FLAG, Move::QUEEN_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }


    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenTwo_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeWhite_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenThree);
        moveGenerator.getBoard().setRookHMoved(true);
        moveGenerator.genCastlingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::QUEEN_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeBlack_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenThree);
        moveGenerator.getBoard().setRookAMoved(false);
        moveGenerator.genCastlingMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::KING_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenStartingPos_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteKingMoved_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setKingMoved(true);
        moveGenerator.genCastlingMoves(true);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackKingMoved_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setKingMoved(false);
        moveGenerator.genCastlingMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookAMoved_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setRookAMoved(true);
        moveGenerator.genCastlingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::KING_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookAMoved_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setRookAMoved(false);
        moveGenerator.genCastlingMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::KING_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookHMoved_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setRookHMoved(true);
        moveGenerator.genCastlingMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::QUEEN_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookHMoved_ShouldReturn1Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.getBoard().setRookHMoved(false);
        moveGenerator.genCastlingMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;

        insertExpectedMoves(expectedMoves, 0, {0}, {Move::QUEEN_CASTLE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }
}
