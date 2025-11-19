// #include <gtest/gtest.h>

// #include "base_gen_test.h"


//     class MoveGeneratorCastlingTest : public BaseGenerator {
//         protected:
//             std::string fenOne;
//             std::string fenTwo;
//             std::string fenThree;
//             std::string bugFen;
//             std::string castlingWhileInCheckWhiteFen;
//             std::string castlingWhileInCheckBlackFen;
//             std::string castlingThroughAttacksWhiteFen;
//             std::string castlingThroughAttacksBlackFen;

//             void SetUp() override {
//                 BaseGenerator::SetUp();
//                 fenOne = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
//                 fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";
//                 bugFen = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 castlingWhileInCheckWhiteFen = "r3k2r/4q3/8/8/8/8/8/R3K2R";
//                 castlingWhileInCheckBlackFen = "r3k2r/8/8/8/8/8/4Q3/R3K2R";
//                 castlingThroughAttacksWhiteFen = "r3k2r/3r1r2/8/8/8/8/8/R3K2R";
//                 castlingThroughAttacksBlackFen = "r3k2r/8/8/8/8/8/3R1R2/R3K2R";
//             }
//     };

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhite_ShouldReturn2Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;
//         insertExpectedMoves(expectedMoves, 0, {0, 0}, {model::Move::KING_CASTLE_FLAG, model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlack_ShouldReturn2Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;
//         insertExpectedMoves(expectedMoves, 0, {0, 0}, {model::Move::KING_CASTLE_FLAG, model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }


//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenTwo_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(fenTwo, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);
//         moveGenerator.gen_castle_moves(false, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeWhite_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenThree, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookHMoved(true, true);
//         moveGenerator.gen_castle_moves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeBlack_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenThree, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookAMoved(false, true);
//         moveGenerator.gen_castle_moves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenStartingPos_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(startingPos, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);
//         moveGenerator.gen_castle_moves(false, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteKingMoved_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setKingMoved(true, true);
//         moveGenerator.gen_castle_moves(true, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackKingMoved_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setKingMoved(false, true);
//         moveGenerator.gen_castle_moves(false, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookAMoved_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookAMoved(true, true);
//         moveGenerator.gen_castle_moves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookAMoved_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookAMoved(false, true);
//         moveGenerator.gen_castle_moves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookHMoved_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookHMoved(true, true);
//         moveGenerator.gen_castle_moves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookHMoved_ShouldReturn1Moves) {
//         io::Fen::set_board_from_fen(fenOne, bitboards, state_bitmasks, piece_map);
//         moveGenerator.getBoard().setRookHMoved(false, true);
//         moveGenerator.gen_castle_moves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.get_move_index(); i++) {
//             auto found = expectedMoves.find(movelist.get_move_at(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_bugFenWhite_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(bugFen, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingWhileInCheckWhiteFen_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(castlingWhileInCheckWhiteFen, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingWhileInCheckBlackFen_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(castlingWhileInCheckBlackFen, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(false, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//         }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingThroughAttacksWhiteFenWhite_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(castlingThroughAttacksWhiteFen, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(true, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingThroughAttacksBlackFenBlack_ShouldReturn0Moves) {
//         io::Fen::set_board_from_fen(castlingThroughAttacksBlackFen, bitboards, state_bitmasks, piece_map);
//         moveGenerator.gen_castle_moves(false, movelist);

//         ASSERT_EQ(movelist.get_move_index(), 0);
//     }
// }
