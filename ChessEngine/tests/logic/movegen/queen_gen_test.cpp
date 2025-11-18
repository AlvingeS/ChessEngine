#include <gtest/gtest.h>

#include "base_gen_test.h"

namespace logic {

class MoveGeneratorQueenTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5k2/2P5/5q2/b4N2/1q6/2Q2Q2/8/8";
    }
};

TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneBlack_ShouldReturnXMoves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, state_bitmasks, piece_map);
    moveGenerator.genQueenMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 42, {50, 49, 56, 41, 40, 33, 24, 34, 35, 28, 21, 43, 44, 45, 46, 47, 51, 60, 50}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0});
    insertExpectedMoves(expectedMoves, 30, {38, 46, 54, 62, 37, 44, 51, 29, 28, 27, 26, 25, 24, 21, 22, 14, 6, 23, 31}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.get_move_index(); i++) {
        auto found = expectedMoves.find(movelist.get_move_at(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneWhite_ShouldReturnXMoves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, state_bitmasks, piece_map);
    moveGenerator.genQueenMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 28, 35, 42, 20, 19, 12, 3, 13, 5, 14, 7, 22, 23, 30}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    insertExpectedMoves(expectedMoves, 18, {26, 25, 32, 17, 16, 9, 0, 10, 2, 11, 4, 19, 20, 27, 36, 45, 54, 63}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.get_move_index(); i++) {
        auto found = expectedMoves.find(movelist.get_move_at(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, state_bitmasks, piece_map);
    moveGenerator.genQueenMoves(true, movelist);

    ASSERT_EQ(movelist.get_move_index(), 0);
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, state_bitmasks, piece_map);
    moveGenerator.genQueenMoves(false, movelist);

    ASSERT_EQ(movelist.get_move_index(), 0);
}

} // namespace logic
