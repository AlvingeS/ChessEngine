#include <gtest/gtest.h>
#include "BaseMoveGeneratorTest.h"

namespace game {

    class MoveGeneratorPawnTest : public BaseMoveGeneratorTest {
        protected:
            std::string fenOne;
            std::string fenEnPessantForWhiteTest;
            std::string fenEnPessantForBlackTest;

            void SetUp() override {
                BaseMoveGeneratorTest::SetUp();
                fenOne = "8/5p1p/5R1p/2p5/3P4/p7/5P1P/8";
                fenEnPessantForWhiteTest = "rnbqkb1r/pppp1ppp/5n2/3Pp3/8/8/8/RNBQKBNR";
                fenEnPessantForBlackTest = "rnbqkbnr/8/8/8/Pp6/8/1PPPPPPP/RNBQKBNR";
            }
    };

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosWhite_ShouldReturn16Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genPawnMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, 2});
        insertExpectedMoves(expectedMoves, 9, {17, 25}, {0, 2});
        insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, 2});
        insertExpectedMoves(expectedMoves, 11, {19, 27}, {0, 2});
        insertExpectedMoves(expectedMoves, 12, {20, 28}, {0, 2});
        insertExpectedMoves(expectedMoves, 13, {21, 29}, {0, 2});
        insertExpectedMoves(expectedMoves, 14, {22, 30}, {0, 2});
        insertExpectedMoves(expectedMoves, 15, {23, 31}, {0, 2});

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
        insertExpectedMoves(expectedMoves, 48, {40, 32}, {0, 2});
        insertExpectedMoves(expectedMoves, 49, {41, 33}, {0, 2});
        insertExpectedMoves(expectedMoves, 50, {42, 34}, {0, 2});
        insertExpectedMoves(expectedMoves, 51, {43, 35}, {0, 2});
        insertExpectedMoves(expectedMoves, 52, {44, 36}, {0, 2});
        insertExpectedMoves(expectedMoves, 53, {45, 37}, {0, 2});
        insertExpectedMoves(expectedMoves, 54, {46, 38}, {0, 2});
        insertExpectedMoves(expectedMoves, 55, {47, 39}, {0, 2});

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
        insertExpectedMoves(expectedMoves, 28, {37, 36}, {1, 0});
        insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, 2});
        insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, 2});

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
        insertExpectedMoves(expectedMoves, 40, {32}, {0});
        insertExpectedMoves(expectedMoves, 37, {28, 29}, {1, 0});
        insertExpectedMoves(expectedMoves, 23, {15}, {0});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForWhite_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenEnPessantForWhiteTest);
        moveGenerator.getBoard().setEnPessantTargetAtIndex(43);
        moveGenerator.genPawnMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 36, {44, 43}, {0, Move::EP_CAPTURE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForBlack_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenEnPessantForBlackTest);
        moveGenerator.getBoard().setEnPessantTargetAtIndex(23);
        moveGenerator.genPawnMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 30, {22, 23}, {0, Move::EP_CAPTURE_FLAG});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }
}
