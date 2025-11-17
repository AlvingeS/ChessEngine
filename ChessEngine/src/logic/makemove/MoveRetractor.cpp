#include "logic/makemove/MoveRetractor.h"

#include "model/position/Board.h"
#include "model/move/Move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : _bitboards(board.bitboards), 
    _stateBitmasks(board.stateBitmasks), 
    _pieceMap(board.pieceMap), 
    _zHasher(board.zHasher)
{}

void MoveRetractor::unmakeCastleMove(bool wasWhite, bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (wasWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboards.clearWhiteKingBit(toKingInd);
        _bitboards.setWhiteKingBit(fromKingInd);
        _bitboards.clearWhiteRooksBit(toRookInd);
        _bitboards.setWhiteRooksBit(fromRookInd);

        _stateBitmasks.setWhitePiecesBit(fromKingInd);
        _stateBitmasks.clearWhitePiecesBit(toKingInd);
        _stateBitmasks.setWhitePiecesBit(fromRookInd);
        _stateBitmasks.clearWhitePiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::W_KING);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::W_ROOK);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboards.setBlackKingBit(fromKingInd);
        _bitboards.clearBlackKingBit(toKingInd);
        _bitboards.setBlackRooksBit(fromRookInd);
        _bitboards.clearBlackRooksBit(toRookInd);

        _stateBitmasks.setBlackPiecesBit(fromKingInd);
        _stateBitmasks.clearBlackPiecesBit(toKingInd);
        _stateBitmasks.setBlackPiecesBit(fromRookInd);
        _stateBitmasks.clearBlackPiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::B_KING);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::B_ROOK);
    }

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveRetractor::unmakeTemporaryKingMove(bool wasWhite, bool isKingSide) 
{
    int from = isKingSide ? (wasWhite ? 2 : 58) 
                          : (wasWhite ? 4 : 60);

    int to = wasWhite ? 3 : 59;

    if (wasWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}

void MoveRetractor::removePreviouslyMovedPieceFromBoard(
    const model::Move& move,
    int toIndex,
    model::Piece::Type previouslyMovedPieceType,
    bool wasWhite) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the placeBackCapturedPieceOnBoard method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboards.clearPieceTypeBit(toIndex, previouslyMovedPieceType);
    } else {
        model::Piece::Type promotionPieceType = MoveUtils::getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboards.clearPieceTypeBit(toIndex, promotionPieceType);
    }

    wasWhite ? _stateBitmasks.clearWhitePiecesBit(toIndex) 
             : _stateBitmasks.clearBlackPiecesBit(toIndex);
}


void MoveRetractor::placeBackCapturedPieceOnBoard(
    bool isEP,
    int captureIndex,
    int toIndex,
    bool wasWhite,
    model::Piece::Type previouslyCapturedPieceType) 
{
    _bitboards.setPieceTypeBit(captureIndex, previouslyCapturedPieceType);
    _pieceMap.setPieceTypeAtIndex(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        _pieceMap.setPieceTypeAtIndex(toIndex, model::Piece::Type::EMPTY);
    }

    wasWhite ? _stateBitmasks.setBlackPiecesBit(captureIndex) 
             : _stateBitmasks.setWhitePiecesBit(captureIndex);
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    model::Piece::Type  movedPieceType)
{
    _bitboards.setPieceTypeBit(fromIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(fromIndex, movedPieceType);

    wasWhite ? _stateBitmasks.setWhitePiecesBit(fromIndex) 
             : _stateBitmasks.setBlackPiecesBit(fromIndex);
}

model::Piece::Type MoveRetractor::determineMovedPieceType(
    const model::Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    model::Piece::Type  previouslyMovedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previouslyMovedPieceType = move.isAnyPromo() ? (wasWhite ? model::Piece::Type::W_PAWN
                                                             : model::Piece::Type::B_PAWN)
                                                 : _pieceMap.getPieceTypeAtIndex(toIndex);

    return previouslyMovedPieceType;
}

void MoveRetractor::unmakeMove(
    const model::Move& previousMove, 
    bool wasWhite, 
    MoveResult previousMoveResults) 
{
    // If the move is a castle, update the bitboards and return
    if (previousMove.isAnyCastle()) {
        unmakeCastleMove(wasWhite, previousMove.isKingCastle());
        return;
    }

    // Get the from and to indices
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    int fromIndex = previousMove.getBitIndexFrom();
    int toIndex = previousMove.getBitIndexTo();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.isAnyCapture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(previousMove, wasWhite, toIndex);

        placeBackCapturedPieceOnBoard(previousMove.isEpCapture(), captureIndex, toIndex, wasWhite, previousMoveResults.capturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        _pieceMap.setPieceTypeAtIndex(toIndex, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

} // namespace logic