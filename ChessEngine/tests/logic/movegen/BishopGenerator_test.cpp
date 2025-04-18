#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorBishopTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5P2/4b3/1P6/2B2B2/1r3b2/8/8/8";
    }
};

TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneWhite_ShouldReturn18Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 34, {41, 48, 25, 16, 27, 20, 13, 6, 43, 52, 61}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    insertExpectedMoves(expectedMoves, 37, {44, 51, 28, 19, 10, 1, 30}, {0, 1, 0, 0, 0, 0, 1});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneBlack_ShouldReturn18Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 51, {58, 42, 33, 24, 44, 37, 60}, {1, 0, 0, 0, 0, 1, 0});
    insertExpectedMoves(expectedMoves, 26, {33, 40, 17, 8, 19, 12, 5, 35, 44, 53, 62}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(false, movelist);;

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

} // namespace logic
