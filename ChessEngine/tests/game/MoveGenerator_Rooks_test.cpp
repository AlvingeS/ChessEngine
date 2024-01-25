#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "8/2P5/8/8/8/b1R5/4q3/8";
                fenTwo = "7r/8/8/1P6/8/1R1p3R/8/8";
            }
    };

    TEST_F(MoveGeneratorTest, genRookMoves_fenOneWhite_ShouldReturn12Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        std::unordered_set<int> expectedToBitIndices = {29, 37, 45, 16, 17, 18, 19, 20, 5, 13, 23, 22}; 
        int expectedBitIndexFrom = 21;

        for (Move move : moves) {
            int bitIndexTo = move.getBitIndexTo();
            ASSERT_EQ(move.getBitIndexFrom(), expectedBitIndexFrom);
            ASSERT_TRUE(expectedToBitIndices.find(bitIndexTo) != expectedToBitIndices.end());
        }
    }

    TEST_F(MoveGeneratorTest, genRookMoves_fenOneBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorTest, genRookMoves_startingPosWhite_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }

    TEST_F(MoveGeneratorTest, genRookMoves_startingPosBlack_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genRookMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moves.size(), 0);
    }
}
