#include "ChessEngine/src/logic/makemove/MoveRetractor.h"

#include "ChessEngine/src/logic/makemove/MoveUtils.h"

#include "ChessEngine/src/model/board/Board.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board,
    engine::SearchMemory& searchMemory
) : _bitboards(board.bitboards), 
    _stateBitmasks(board.stateBitmasks), 
    _pieceMap(board.pieceMap), 
    _zHasher(board.zHasher),
    _searchMemory(searchMemory) 
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

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::PieceType::W_KING);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::PieceType::W_ROOK);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::PieceType::EMPTY);
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

        _pieceMap.setPieceTypeAtIndex(toKingInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::PieceType::B_KING);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::PieceType::B_ROOK);
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
    model::PieceType previouslyMovedPieceType,
    bool wasWhite) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the placeBackCapturedPieceOnBoard method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboards.clearPieceTypeBit(toIndex, previouslyMovedPieceType);
    } else {
        model::PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), wasWhite);
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
    model::PieceType previouslyCapturedPieceType) 
{
    _bitboards.setPieceTypeBit(captureIndex, previouslyCapturedPieceType);
    _pieceMap.setPieceTypeAtIndex(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        _pieceMap.setPieceTypeAtIndex(toIndex, model::PieceType::EMPTY);
    }

    wasWhite ? _stateBitmasks.setBlackPiecesBit(captureIndex) 
             : _stateBitmasks.setWhitePiecesBit(captureIndex);
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    model::PieceType  movedPieceType)
{
    _bitboards.setPieceTypeBit(fromIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(fromIndex, movedPieceType);

    wasWhite ? _stateBitmasks.setWhitePiecesBit(fromIndex) 
             : _stateBitmasks.setBlackPiecesBit(fromIndex);
}

model::PieceType MoveRetractor::determineMovedPieceType(
    const model::Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    model::PieceType  previouslyMovedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previouslyMovedPieceType = move.isAnyPromo() ? (wasWhite ? model::PieceType::W_PAWN
                                                             : model::PieceType::B_PAWN)
                                                 : _pieceMap.getPieceTypeAtIndex(toIndex);

    return previouslyMovedPieceType;
}

void MoveRetractor::unmakeMove(
    const model::Move& previousMove, 
    bool wasWhite, 
    int currentDepth) 
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
    assert(fromIndex != toIndex);
    
    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::PieceType  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.isAnyCapture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = determineCaptureIndex(previousMove, wasWhite, toIndex);
        model::PieceType  previouslyCapturedPieceType = _searchMemory.getLastCapturedPieceAtDepth(currentDepth);

        placeBackCapturedPieceOnBoard(previousMove.isEpCapture(), captureIndex, toIndex, wasWhite, previouslyCapturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        _pieceMap.setPieceTypeAtIndex(toIndex, model::PieceType::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    if (previousMove.isDoublePawnPush()) {
        _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }

    if (not previousMove.isAnyCapture() && (previouslyMovedPieceType !=model::PieceType::W_PAWN && previouslyMovedPieceType !=model::PieceType::B_PAWN)) {
        _searchMemory.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

} // namespace logic