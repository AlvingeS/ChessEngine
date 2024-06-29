// #include "ChessEngine/movegen/MoveGenerator.h"
// #include "ChessEngine/game/Move.h"
// #include <gtest/gtest.h>
// #include <unordered_set>
// #include <vector>
// #include <unordered_map>

// namespace game {

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
//             std::vector<Move> moveList;

//             MakeUnmakeMoveTest() : board(ChessBoard()), moveGenerator(MoveGenerator(board)) {}

//             virtual void SetUp() override {
//                 castling = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 captures = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 normalPromotion = "8/1P6/8/8/8/8/6p1/8";
//                 capturePromotion = "p1b5/1P6/8/8/8/8/6p1/5N1Q";
//                 startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//                 enPessant = "8/4p3/8/3P4/7p/8/6P1/8";
//                 moveList = std::vector<Move>(movegen::MoveGenerator::MAX_LEGAL_MOVES);
//             }

//             std::vector<Move> getMoves() {
//                 return moveList;
//             }

//             std::vector<U64> getBitboards() {
//                 return moveGenerator.getBoard().getBitboards();
//             }

//             std::vector<PieceType> getSquaresLookup() {
//                 return moveGenerator.getBoard().getSquaresLookup();
//             }
//     };

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionWhite) {
//         moveGenerator.genMoves(true, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = moveList;

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//         TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionBlack) {
//         moveGenerator.genMoves(false, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }

//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesWhite) {
//         moveGenerator.setBoardFromFen(captures);
//         moveGenerator.genMoves(true, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesBlack) {
//         moveGenerator.setBoardFromFen(captures);
//         moveGenerator.genMoves(false, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingWhite) {
//         moveGenerator.setBoardFromFen(castling);
//         moveGenerator.genMoves(true, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingBlack) {
//         moveGenerator.setBoardFromFen(castling);
//         moveGenerator.genMoves(false, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionWhite) {
//         moveGenerator.setBoardFromFen(normalPromotion);
//         moveGenerator.genMoves(true, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionBlack) {
//         moveGenerator.setBoardFromFen(normalPromotion);
//         moveGenerator.genMoves(false, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionWhite) {
//         moveGenerator.setBoardFromFen(capturePromotion);
//         moveGenerator.genMoves(true, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionBlack) {
//         moveGenerator.setBoardFromFen(capturePromotion);
//         moveGenerator.genMoves(false, moveList);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantWhite) {
//         moveGenerator.setBoardFromFen(enPessant);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(51, 35, Move::DOUBLE_PAWN_PUSH_FLAG), false);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
        
//         moveGenerator.genMoves(true, moveList);
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantBlack) {
//         moveGenerator.setBoardFromFen(enPessant);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(9, 17, Move::DOUBLE_PAWN_PUSH_FLAG), true);

//         // Makes copies of the bitboards and squaresLookup
//         std::vector<U64> bitboardsCopy = getBitboards();
//         std::vector<PieceType> squaresLookupCopy = getSquaresLookup();
        
//         moveGenerator.genMoves(false, moveList);
//         std::vector<Move> moves = getMoves();

//         for (size_t i = 0; i < moveGenerator.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getSquaresLookup(), squaresLookupCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsWhite) {
//         moveGenerator.setBoardFromFen(castling);

//         moveGenerator.genCastlingMoves(true, moveList);
//         std::vector<Move> castlingMoves = getMoves();

//         Move whiteHRookMove = Move(0, 8, 0);
//         Move whiteARookMove = Move(7, 15, 0);
//         Move whiteKingMove = Move(3, 2, 0);

//         moveGenerator.getBoard().makeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().makeMove(whiteARookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteARookMove, true);
//         moveGenerator.getBoard().makeMove(whiteKingMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteKingMove, true);

//         moveGenerator.resetMoves(moveList);
//         moveGenerator.genCastlingMoves(true, moveList);
//         std::vector<Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsBlack) {
//         moveGenerator.setBoardFromFen(castling);

//         moveGenerator.genCastlingMoves(false, moveList);
//         std::vector<Move> castlingMoves = getMoves();

//         Move blackHRookMove = Move(56, 48, 0);
//         Move blackARookMove = Move(63, 55, 0);
//         Move blackKingMove = Move(59, 58, 0);

//         moveGenerator.getBoard().makeMove(blackHRookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackHRookMove, false);
//         moveGenerator.getBoard().makeMove(blackARookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackARookMove, false);
//         moveGenerator.getBoard().makeMove(blackKingMove, false);
//         moveGenerator.getBoard().unmakeMove(blackKingMove, false);

//         moveGenerator.resetMoves(moveList);
//         moveGenerator.genCastlingMoves(false, moveList);
//         std::vector<Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }
// }

