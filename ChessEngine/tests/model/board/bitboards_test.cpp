#include <gtest/gtest.h>

#include "model/position/board.h"

#include "io/fen.h"

namespace model {

class BitboardsTest : public ::testing::Test 
{
protected:
    Bitboards bbs;
    OccupancyMasks occupancy_masks;
    PieceMap piece_map;

    std::string fenOne;

    BitboardsTest()
        : bbs(), 
        occupancy_masks(bbs), 
        piece_map(bbs), 
        fenOne("8/8/8/8/3Pp3/8/8/8") 
    {
        // Constructor body if needed
    }

    void SetUp() override 
    {
        // Additional setup if needed, but member variables are already initialized
    }
};


TEST_F(BitboardsTest, ConstructorInit_WhitePawn) 
{
    bitboard expected = 0x000000000000FF00ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKnight) 
{
    bitboard expected = 0x0000000000000042ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteBishop) 
{
    bitboard expected = 0x0000000000000024ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteRook) 
{
    bitboard expected = 0x0000000000000081ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteQueen) 
{
    bitboard expected = 0x0000000000000010ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKing) 
{
    bitboard expected = 0x0000000000000008ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::W_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackPawn) 
{
    bitboard expected = 0x00FF000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKnight) 
{
    bitboard expected = 0x4200000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackBishop) 
{
    bitboard expected = 0x2400000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackRook) 
{
    bitboard expected = 0x8100000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackQueen) 
{
    bitboard expected = 0x1000000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKing) 
{
    bitboard expected = 0x0800000000000000ULL;
    bitboard actual = bbs.get_bb_from_piece_type(Piece::Type::B_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getWhitePiecesbitboard_ShouldReturn0x000000000000FFFF) 
{
    bitboard expected = 0x000000000000FFFFULL;
    bitboard actual = occupancy_masks.get_w_pieces_mask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getBlackPiecesbitboard_ShouldReturn0xFFFF000000000000) 
{
    bitboard expected = 0xFFFF000000000000ULL;
    bitboard actual = occupancy_masks.get_b_pieces_mask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, fillPieceMapTest_ShouldReturnStartingPos) 
{
    ASSERT_EQ(piece_map.get_piece_type_at(0), Piece::Type::W_ROOK);
    ASSERT_EQ(piece_map.get_piece_type_at(1), Piece::Type::W_KNIGHT);
    ASSERT_EQ(piece_map.get_piece_type_at(2), Piece::Type::W_BISHOP);
    ASSERT_EQ(piece_map.get_piece_type_at(3), Piece::Type::W_KING);
    ASSERT_EQ(piece_map.get_piece_type_at(4), Piece::Type::W_QUEEN);
    ASSERT_EQ(piece_map.get_piece_type_at(5), Piece::Type::W_BISHOP);
    ASSERT_EQ(piece_map.get_piece_type_at(6), Piece::Type::W_KNIGHT);
    ASSERT_EQ(piece_map.get_piece_type_at(7), Piece::Type::W_ROOK);

    for (int i = 8; i < 16; i++) {
        ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::W_PAWN);
    }

    for (int i = 16; i < 48; i++) {
        ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::EMPTY);
    }

    for (int i = 48; i < 56; i++) {
        ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::B_PAWN);
    }

    ASSERT_EQ(piece_map.get_piece_type_at(56), Piece::Type::B_ROOK);
    ASSERT_EQ(piece_map.get_piece_type_at(57), Piece::Type::B_KNIGHT);
    ASSERT_EQ(piece_map.get_piece_type_at(58), Piece::Type::B_BISHOP);
    ASSERT_EQ(piece_map.get_piece_type_at(59), Piece::Type::B_KING);
    ASSERT_EQ(piece_map.get_piece_type_at(60), Piece::Type::B_QUEEN);
    ASSERT_EQ(piece_map.get_piece_type_at(61), Piece::Type::B_BISHOP);
    ASSERT_EQ(piece_map.get_piece_type_at(62), Piece::Type::B_KNIGHT);
    ASSERT_EQ(piece_map.get_piece_type_at(63), Piece::Type::B_ROOK);
}

TEST_F(BitboardsTest, fillPieceMapFenOneTest_ShouldReturnTwoPieces) 
{
    io::fen::set_board_from_fen(fenOne, bbs, occupancy_masks, piece_map);

    for (int i = 0; i < 64; i++) {
        if (i == 27) {
            ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::B_PAWN);
        } else if (i == 28) {
            ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::W_PAWN);
        } else {
            ASSERT_EQ(piece_map.get_piece_type_at(i), Piece::Type::EMPTY);
        }
    }
}

} // namespace model
