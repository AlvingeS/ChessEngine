// #include <gtest/gtest.h>

// #include "ChessEngine/src/game/movegen/MoveGenerator.h"
// #include "ChessEngine/src/game/move/Move.h"

// #include <unordered_set>
// #include <vector>
// #include <unordered_map>

// namespace logic {

//     class MakeUnmakeMoveTest : public ::testing::Test {
//         protected:
//             ChessBoard board;
//             MoveGenerator moveGenerator;
//             std::string startingPos;
//             std::string castling;
//             std::string captures;
//             std::string normalPromotion;
//             std::string capturePromotion;
//             std::string enPessant;
//             std::vector<model::Move> movelist;

//             MakeUnmakeMoveTest() : board(ChessBoard()), moveGenerator(MoveGenerator(board)) {}

//             virtual void SetUp() override {
//                 castling = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 captures = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 normalPromotion = "8/1P6/8/8/8/8/6p1/8";
//                 capturePromotion = "p1b5/1P6/8/8/8/8/6p1/5N1Q";
//                 startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//                 enPessant = "8/4p3/8/3P4/7p/8/6P1/8";
//                 movelist = std::vector<model::Move>(movegen::MoveGenerator::MAX_LEGAL_MOVES);
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
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = movelist;

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//         TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionBlack) {
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }

//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesWhite) {
//         io::Fen::setBoardFromFen(captures, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesBlack) {
//         io::Fen::setBoardFromFen(captures, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingWhite) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingBlack) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionWhite) {
//         io::Fen::setBoardFromFen(normalPromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionBlack) {
//         io::Fen::setBoardFromFen(normalPromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionWhite) {
//         io::Fen::setBoardFromFen(capturePromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionBlack) {
//         io::Fen::setBoardFromFen(capturePromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantWhite) {
//         io::Fen::setBoardFromFen(enPessant, bitboards, stateBitmasks, pieceMap);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(51, 35, model::Move::DOUBLE_PAWN_PUSH_FLAG), false);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.genMoves(true, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantBlack) {
//         io::Fen::setBoardFromFen(enPessant, bitboards, stateBitmasks, pieceMap);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(9, 17, model::Move::DOUBLE_PAWN_PUSH_FLAG), true);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.genMoves(false, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsWhite) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);

//         moveGenerator.genCastlingMoves(true, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move whiteHRookMove = model::Move(0, 8, 0);
//         Move whiteARookMove = model::Move(7, 15, 0);
//         Move whiteKingMove = model::Move(3, 2, 0);

//         moveGenerator.getBoard().makeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().makeMove(whiteARookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteARookMove, true);
//         moveGenerator.getBoard().makeMove(whiteKingMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteKingMove, true);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.genCastlingMoves(true, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsBlack) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);

//         moveGenerator.genCastlingMoves(false, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move blackHRookMove = model::Move(56, 48, 0);
//         Move blackARookMove = model::Move(63, 55, 0);
//         Move blackKingMove = model::Move(59, 58, 0);

//         moveGenerator.getBoard().makeMove(blackHRookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackHRookMove, false);
//         moveGenerator.getBoard().makeMove(blackARookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackARookMove, false);
//         moveGenerator.getBoard().makeMove(blackKingMove, false);
//         moveGenerator.getBoard().unmakeMove(blackKingMove, false);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.genCastlingMoves(false, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }
// }

