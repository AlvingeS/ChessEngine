#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace game {
namespace movegen {

class MoveGeneratorPawnTest : public BaseGenerator 
{
protected:
    std::string fenOne;
    std::string fenEnPessantForWhiteTest;
    std::string fenEnPessantForBlackTest;
    std::string fenPromotionTest;
    std::string fenBuggedPawnTest;
    // ponder::perft::SearchMemory searchMemory;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "8/5p1p/5R1p/2p5/3P4/p7/5P1P/8";
        fenEnPessantForWhiteTest = "rnbqkb1r/pppp1ppp/5n2/3Pp3/8/8/8/RNBQKBNR";
        fenEnPessantForBlackTest = "rnbqkbnr/8/8/8/Pp6/8/1PPPPPPP/RNBQKBNR";
        fenPromotionTest = "3q4/2P3P1/8/8/8/8/1p5p/2N5";
        fenBuggedPawnTest = "rnbqkb1r/pppppppp/7n/8/8/2N4N/PPPPPPPP/R1BQKB1R";
        // searchMemory = ponder::perft::SearchMemory(0);
    }
};

TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosWhite_ShouldReturn16Moves) 
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(true, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 9, {17, 25}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 11, {19, 27}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 12, {20, 28}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 13, {21, 29}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 14, {22, 30}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 15, {23, 31}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosBlack_ShouldReturn16Moves) 
{
    utils::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(false, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 48, {40, 32}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 49, {41, 33}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 50, {42, 34}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 51, {43, 35}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 52, {44, 36}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 53, {45, 37}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 54, {46, 38}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 55, {47, 39}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneWhite_ShouldReturn6Moves) 
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(true, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 28, {37, 36}, {1, 0});
    insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneBlack_ShouldReturnXMoves) 
{
    utils::setBoardFromFen(fenOne, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(false, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 40, {32}, {0});
    insertExpectedMoves(expectedMoves, 37, {28, 29}, {1, 0});
    insertExpectedMoves(expectedMoves, 23, {15}, {0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForWhite_ShouldReturn2Moves) 
{
    utils::setBoardFromFen(fenEnPessantForWhiteTest, bitboards, gameStateBitmasks, squaresLookup);
    // moveGenerator.getBoard().setEnPessantTargetAtIndex(43);
    moveGenerator.genPawnMoves(true, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 36, {44, 43}, {0, Move::EP_CAPTURE_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForBlack_ShouldReturn2Moves) 
{
    utils::setBoardFromFen(fenEnPessantForBlackTest, bitboards, gameStateBitmasks, squaresLookup);
    // moveGenerator.getBoard().setEnPessantTargetAtIndex(23);
    moveGenerator.genPawnMoves(false, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 30, {22, 23}, {0, Move::EP_CAPTURE_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenPromotionTestWhite_ShouldReturn12Moves) 
{
    utils::setBoardFromFen(fenPromotionTest, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(true, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 53, {61, 61, 61, 61}, {Move::KNIGHT_PROMO_FLAG, Move::BISHOP_PROMO_FLAG, Move::ROOK_PROMO_FLAG, Move::QUEEN_PROMO_FLAG});
    insertExpectedMoves(expectedMoves, 53, {60, 60, 60, 60}, {Move::KNIGHT_PROMO_CAPTURE_FLAG, Move::BISHOP_PROMO_CAPTURE_FLAG, Move::ROOK_PROMO_CAPTURE_FLAG, Move::QUEEN_PROMO_CAPTURE_FLAG});
    insertExpectedMoves(expectedMoves, 49, {57, 57, 57, 57}, {Move::KNIGHT_PROMO_FLAG, Move::BISHOP_PROMO_FLAG, Move::ROOK_PROMO_FLAG, Move::QUEEN_PROMO_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenPromotionTestBlack_ShouldReturn12Moves) 
{
    utils::setBoardFromFen(fenPromotionTest, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(false, movelist, 0, searchMemory);

    Movelist moves = getMoves();
    std::unordered_set<Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 8, {0, 0, 0, 0}, {Move::KNIGHT_PROMO_FLAG, Move::BISHOP_PROMO_FLAG, Move::ROOK_PROMO_FLAG, Move::QUEEN_PROMO_FLAG});
    insertExpectedMoves(expectedMoves, 14, {5, 5, 5, 5}, {Move::KNIGHT_PROMO_CAPTURE_FLAG, Move::BISHOP_PROMO_CAPTURE_FLAG, Move::ROOK_PROMO_CAPTURE_FLAG, Move::QUEEN_PROMO_CAPTURE_FLAG});
    insertExpectedMoves(expectedMoves, 14, {6, 6, 6, 6}, {Move::KNIGHT_PROMO_FLAG, Move::BISHOP_PROMO_FLAG, Move::ROOK_PROMO_FLAG, Move::QUEEN_PROMO_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenBuggedPawnTestWhite_ShouldNotReturnh6) 
{
    utils::setBoardFromFen(fenBuggedPawnTest, bitboards, gameStateBitmasks, squaresLookup);
    moveGenerator.genPawnMoves(false, movelist, 0, searchMemory);

    Movelist moves = getMoves();

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        ASSERT_NE(movelist.getMoveAt(i), Move(46, 38, Move::QUITE_FLAG));
    }
}

} // namespace movegen
} // namespace game
