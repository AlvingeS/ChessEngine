// #include <gtest/gtest.h>

// #include "game/movegen/MoveGen.h"
// #include "game/move/Move.h"

// #include <unordered_set>
// #include <vector>
// #include <unordered_map>

// namespace logic {

//     class MakeUnmakeMoveTest : public ::testing::Test {
//         protected:
//             ChessBoard board;
//             MoveGen moveGenerator;
//             std::string startingPos;
//             std::string castling;
//             std::string captures;
//             std::string normalPromotion;
//             std::string capturePromotion;
//             std::string enPessant;
//             std::vector<model::Move> movelist;

//             MakeUnmakeMoveTest() : board(ChessBoard()), moveGenerator(MoveGen(board)) {}

//             virtual void SetUp() override {
//                 castling = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 captures = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 normalPromotion = "8/1P6/8/8/8/8/6p1/8";
//                 capturePromotion = "p1b5/1P6/8/8/8/8/6p1/5N1Q";
//                 startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//                 enPessant = "8/4p3/8/3P4/7p/8/6P1/8";
//                 movelist = std::vector<model::Move>(movegen::MoveGen::MAX_LEGAL_MOVES);
//             }

//             std::vector<model::Move> getMoves() {
//                 return movelist;
//             }

//             std::vector<bitboard> getBitboards() {
//                 return moveGenerator.getBoard().getBitboards();
//             }

//             std::vector<Piece::Type> getPieceMap() {
//                 return moveGenerator.getBoard().getPieceMap();
//             }
//     };

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionWhite) {
//         moveGenerator.gen_moves(true, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = movelist;

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//         TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionBlack) {
//         moveGenerator.gen_moves(false, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }

//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesWhite) {
//         io::fen::set_board_from_fen(captures, pos);
//         moveGenerator.gen_moves(true, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesBlack) {
//         io::fen::set_board_from_fen(captures, pos);
//         moveGenerator.gen_moves(false, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingWhite) {
//         io::fen::set_board_from_fen(castling, pos);
//         moveGenerator.gen_moves(true, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingBlack) {
//         io::fen::set_board_from_fen(castling, pos);
//         moveGenerator.gen_moves(false, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionWhite) {
//         io::fen::set_board_from_fen(normalPromotion, pos);
//         moveGenerator.gen_moves(true, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionBlack) {
//         io::fen::set_board_from_fen(normalPromotion, pos);
//         moveGenerator.gen_moves(false, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionWhite) {
//         io::fen::set_board_from_fen(capturePromotion, pos);
//         moveGenerator.gen_moves(true, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionBlack) {
//         io::fen::set_board_from_fen(capturePromotion, pos);
//         moveGenerator.gen_moves(false, movelist);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantWhite) {
//         io::fen::set_board_from_fen(enPessant, pos);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().make_move(Move(51, 35, model::Move::DOUBLE_PAWN_PUSH_FLAG), false);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.gen_moves(true, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], true);
//             moveGenerator.getBoard().unmake_move(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantBlack) {
//         io::fen::set_board_from_fen(enPessant, pos);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().make_move(Move(9, 17, model::Move::DOUBLE_PAWN_PUSH_FLAG), true);

//         // Makes copies of the bbs and piece_map
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.gen_moves(false, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.get_move_idx(); i++) {
//             moveGenerator.getBoard().make_move(moves[i], false);
//             moveGenerator.getBoard().unmake_move(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsWhite) {
//         io::fen::set_board_from_fen(castling, pos);

//         moveGenerator.gen_castle_moves(true, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move whiteHRookMove = model::Move(0, 8, 0);
//         Move whiteARookMove = model::Move(7, 15, 0);
//         Move whiteKingMove = model::Move(3, 2, 0);

//         moveGenerator.getBoard().make_move(whiteHRookMove, true);
//         moveGenerator.getBoard().unmake_move(whiteHRookMove, true);
//         moveGenerator.getBoard().make_move(whiteARookMove, true);
//         moveGenerator.getBoard().unmake_move(whiteARookMove, true);
//         moveGenerator.getBoard().make_move(whiteKingMove, true);
//         moveGenerator.getBoard().unmake_move(whiteKingMove, true);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.gen_castle_moves(true, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsBlack) {
//         io::fen::set_board_from_fen(castling, pos);

//         moveGenerator.gen_castle_moves(false, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move blackHRookMove = model::Move(56, 48, 0);
//         Move blackARookMove = model::Move(63, 55, 0);
//         Move blackKingMove = model::Move(59, 58, 0);

//         moveGenerator.getBoard().make_move(blackHRookMove, false);
//         moveGenerator.getBoard().unmake_move(blackHRookMove, false);
//         moveGenerator.getBoard().make_move(blackARookMove, false);
//         moveGenerator.getBoard().unmake_move(blackARookMove, false);
//         moveGenerator.getBoard().make_move(blackKingMove, false);
//         moveGenerator.getBoard().unmake_move(blackKingMove, false);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.gen_castle_moves(false, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }
// }

