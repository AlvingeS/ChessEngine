#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace game {
namespace movegen {

class MoveGeneratorKingTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5r2/3qk3/4R3/8/8/8/Pn6/1K6";
    }
};

TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneWhite_ShouldReturn4Moves)
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKingMoves(true, movelist);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 6, {14, 13, 5, 7}, {1, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneBlack_ShouldReturn6Moves)
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKingMoves(false, movelist);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 51, {59, 50, 42, 43, 44, 60}, {0, 0, 0, 1, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosWhite_ShouldReturn0Moves)
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKingMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosBlack_ShouldReturn0Moves)
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKingMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

} // namespace movegen
} // namespace game
