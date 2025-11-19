#include <gtest/gtest.h>

#include "base_gen_test.h"

namespace logic {

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
    io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
    moveGenerator.gen_king_moves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 6, {14, 13, 5, 7}, {1, 0, 0, 0});

    for (int i = 0; i < movelist.get_move_index(); i++) {
        auto found = expectedMoves.find(movelist.get_move_at(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneBlack_ShouldReturn6Moves)
{
    io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
    moveGenerator.gen_king_moves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 51, {59, 50, 42, 43, 44, 60}, {0, 0, 0, 1, 0, 0});

    for (int i = 0; i < movelist.get_move_index(); i++) {
        auto found = expectedMoves.find(movelist.get_move_at(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosWhite_ShouldReturn0Moves)
{
    io::Fen::set_board_from_fen(startingPos, bitboards, state_bitmasks, piece_map);
    moveGenerator.gen_king_moves(true, movelist);

    ASSERT_EQ(movelist.get_move_index(), 0);
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosBlack_ShouldReturn0Moves)
{
    io::Fen::set_board_from_fen(startingPos, bitboards, state_bitmasks, piece_map);
    moveGenerator.gen_king_moves(false, movelist);

    ASSERT_EQ(movelist.get_move_index(), 0);
}

} // namespace logic
