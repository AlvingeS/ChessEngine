#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace game {
namespace movegen {

class MoveGeneratorKnightTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "8/1n1n4/5R2/2N5/4P3/8/5p2/7N";
    }
};

TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosWhite_ShouldReturn4Moves) 
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKnightMoves(true, movelist);

    Movelist moves = getMoves();
    std::unordered_set<move::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 1, {16, 18}, {0, 0});
    insertExpectedMoves(expectedMoves, 6, {21, 23}, {0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosBlack_ShouldReturn4Moves) 
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKnightMoves(false, movelist);

    Movelist moves = getMoves();
    std::unordered_set<move::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 57, {40, 42}, {0, 0});
    insertExpectedMoves(expectedMoves, 62, {45, 47}, {0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneWhite_ShouldReturn8Moves) 
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKnightMoves(true, movelist);

    Movelist moves = getMoves();
    std::unordered_set<move::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 0, {10, 17}, {1, 0});
    insertExpectedMoves(expectedMoves, 37, {52, 43, 20, 22, 31, 47, 54}, {1, 0, 0, 0, 0, 0, 1});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneBlack_ShouldReturn10Moves) 
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genKnightMoves(false, movelist);

    Movelist moves = getMoves();
    std::unordered_set<move::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 52, {58, 42, 35, 37, 46, 62}, {0, 1, 0, 1, 0, 0});
    insertExpectedMoves(expectedMoves, 54, {60, 44, 37, 39}, {0, 0, 1, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

} // namespace movegen
} // namespace game
