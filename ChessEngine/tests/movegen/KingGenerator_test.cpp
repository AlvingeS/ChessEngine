#include <gtest/gtest.h>
#include "BaseGenerator_test.h"

namespace movegen {

    class MoveGeneratorKingTest : public BaseGenerator {
        protected:
            std::string fenOne;

            void SetUp() override {
                BaseGenerator::SetUp();
                fenOne = "5r2/3qk3/4R3/8/8/8/Pn6/1K6";
            }
    };

    TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneWhite_ShouldReturn4Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKingMoves(true, moveList);

        std::vector<game::Move> moves = getMoves();
        std::unordered_set<game::Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 6, {14, 13, 5, 7}, {1, 0, 0, 0});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneBlack_ShouldReturn6Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genKingMoves(false, moveList);

        std::vector<game::Move> moves = getMoves();
        std::unordered_set<game::Move> expectedMoves;
        insertExpectedMoves(expectedMoves, 51, {59, 50, 42, 43, 44, 60}, {0, 0, 0, 1, 0, 0});

        for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
            auto found = expectedMoves.find(moves[i]);
            ASSERT_TRUE(found != expectedMoves.end());
            expectedMoves.erase(found); // Remove found move from the set
        }

        ASSERT_TRUE(expectedMoves.empty());
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKingMoves(true, moveList);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }

    TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genKingMoves(false, moveList);

        ASSERT_EQ(moveGenerator.getMoveIndex(), 0);
    }
}
