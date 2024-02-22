#include <gtest/gtest.h>
#include "BaseMoveGeneratorTest.h"

namespace game {

    class MoveGeneratorCountsTest : public BaseMoveGeneratorTest {
        protected:
            std::string fenOne;

            void SetUp() override {
                BaseMoveGeneratorTest::SetUp();
                fenOne = "8/1n1n4/5R2/2N5/4P3/8/5p2/7N";
            }
    };

    TEST_F(MoveGeneratorCountsTest, genAllMoves_startPosWhite_ShouldReturn20Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moveGenerator.getMoveIndex(), 20);
    }

    TEST_F(MoveGeneratorCountsTest, genAllMoves_startPosBlack_ShouldReturn20Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moveGenerator.getMoveIndex(), 20);
    }
}
