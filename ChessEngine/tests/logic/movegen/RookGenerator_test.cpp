#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorRookTest : public BaseGenerator 
{
    protected:
        std::string fenOne;
        std::string fenTwo;

        void SetUp() override {
            BaseGenerator::SetUp();
            fenOne = "8/2P5/8/8/8/b1R5/4q3/8";
            fenTwo = "7r/8/8/1P6/8/1R1p3R/8/8";
        }
};

TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneWhite_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 16, 17, 18, 19, 20, 5, 13, 23, 22}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}); 

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoWhite_ShouldReturn17Moves) 
{
    io::Fen::setBoardFromFen(fenTwo, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 16, {24, 32, 40, 48, 56, 8, 0, 17, 18, 19, 20}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1});
    insertExpectedMoves(expectedMoves, 22, {30, 21, 20, 14, 6, 23}, {0, 0, 1, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());        
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoBlack_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenTwo, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 56, {48, 40, 32, 24, 16, 57, 58, 59, 60, 61, 62, 63}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());   
}

} // namespace logic
