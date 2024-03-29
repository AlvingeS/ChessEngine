#include <gtest/gtest.h>
#include "BaseMoveGeneratorTest.h"

namespace game {

    class MoveGeneratorRookTest : public BaseMoveGeneratorTest {
        protected:
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                BaseMoveGeneratorTest::SetUp();
                fenOne = "8/2P5/8/8/8/b1R5/4q3/8";
                fenTwo = "7r/8/8/1P6/8/1R1p3R/8/8";
            }
    };

    TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneWhite_ShouldReturn12Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 16, 17, 18, 19, 20, 5, 13, 23, 22}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}); 

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(true);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(false);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoWhite_ShouldReturn17Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 16, {24, 32, 40, 48, 56, 8, 0, 17, 18, 19, 20}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1});
        insertExpectedMoves(expectedMoves, 22, {30, 21, 20, 14, 6, 23}, {0, 0, 1, 0, 0, 0});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());        
    }

    TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoBlack_ShouldReturn12Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 56, {48, 40, 32, 24, 16, 57, 58, 59, 60, 61, 62, 63}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());   
    }
}
