#include "ChessEngine/src/logic/makemove/MoveRetractor.h"

#include "ChessEngine/src/logic/makemove/MoveUtils.h"

namespace logic {
namespace makemove {

MoveRetractor::MoveRetractor(
    representation::board::Bitboards& bitBoards, 
    representation::board::GameStateBitmasks& gameStateBitmasks, 
    representation::board::SquaresLookup& squaresLookup, 
    representation::board::ZHasher& zHasher,
    engine::search::SearchMemory& searchMemory
) : _bitboardsRef(bitBoards), 
    _gameStateBitmasksRef(gameStateBitmasks), 
    _squaresLookupRef(squaresLookup), 
    _searchMemoryRef(searchMemory), 
    _zHasherRef(zHasher)
{}

void MoveRetractor::unmakeCastleMove(bool wasWhite, bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (wasWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboardsRef.clearWhiteKingBit(toKingInd);
        _bitboardsRef.setWhiteKingBit(fromKingInd);
        _bitboardsRef.clearWhiteRooksBit(toRookInd);
        _bitboardsRef.setWhiteRooksBit(fromRookInd);

        _gameStateBitmasksRef.setWhitePiecesBit(fromKingInd);
        _gameStateBitmasksRef.clearWhitePiecesBit(toKingInd);
        _gameStateBitmasksRef.setWhitePiecesBit(fromRookInd);
        _gameStateBitmasksRef.clearWhitePiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd,representation::board::PieceType::W_KING);
        _squaresLookupRef.setPieceTypeAtIndex(toKingInd,representation::board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd,representation::board::PieceType::W_ROOK);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd,representation::board::PieceType::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboardsRef.setBlackKingBit(fromKingInd);
        _bitboardsRef.clearBlackKingBit(toKingInd);
        _bitboardsRef.setBlackRooksBit(fromRookInd);
        _bitboardsRef.clearBlackRooksBit(toRookInd);

        _gameStateBitmasksRef.setBlackPiecesBit(fromKingInd);
        _gameStateBitmasksRef.clearBlackPiecesBit(toKingInd);
        _gameStateBitmasksRef.setBlackPiecesBit(fromRookInd);
        _gameStateBitmasksRef.clearBlackPiecesBit(toRookInd);

        _squaresLookupRef.setPieceTypeAtIndex(toKingInd,representation::board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromKingInd,representation::board::PieceType::B_KING);
        _squaresLookupRef.setPieceTypeAtIndex(toRookInd,representation::board::PieceType::EMPTY);
        _squaresLookupRef.setPieceTypeAtIndex(fromRookInd,representation::board::PieceType::B_ROOK);
    }

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveRetractor::unmakeTemporaryKingMove(bool wasWhite, bool isKingSide) 
{
    int from = isKingSide ? (wasWhite ? 2 : 58) 
                          : (wasWhite ? 4 : 60);

    int to = wasWhite ? 3 : 59;

    if (wasWhite) {
        _bitboardsRef.clearWhiteKingBit(from);
        _bitboardsRef.setWhiteKingBit(to);
    } else {
        _bitboardsRef.clearBlackKingBit(from);
        _bitboardsRef.setBlackKingBit(to);
    }
}

void MoveRetractor::removePreviouslyMovedPieceFromBoard(const representation::move::Move& move, int toIndex,representation::board::PieceType  previouslyMovedPieceType, bool wasWhite) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the placeBackCapturedPieceOnBoard method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboardsRef.clearPieceTypeBit(toIndex, previouslyMovedPieceType);
    } else {
       representation::board::PieceType  promotionPieceType = getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboardsRef.clearPieceTypeBit(toIndex, promotionPieceType);
    }

    if (wasWhite) {
        _gameStateBitmasksRef.clearWhitePiecesBit(toIndex);
    } else {
        _gameStateBitmasksRef.clearBlackPiecesBit(toIndex);
    }
}


void MoveRetractor::placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite,representation::board::PieceType  previouslyCapturedPieceType) 
{
    _bitboardsRef.setPieceTypeBit(captureIndex, previouslyCapturedPieceType);
    _squaresLookupRef.setPieceTypeAtIndex(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        _squaresLookupRef.setPieceTypeAtIndex(toIndex,representation::board::PieceType::EMPTY);
    }

    if (wasWhite) {
        _gameStateBitmasksRef.setBlackPiecesBit(captureIndex);
    } else {
        _gameStateBitmasksRef.setWhitePiecesBit(captureIndex);
    }
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    representation::board::PieceType  movedPieceType)
{
    _bitboardsRef.setPieceTypeBit(fromIndex, movedPieceType);
    _squaresLookupRef.setPieceTypeAtIndex(fromIndex, movedPieceType);

    if (wasWhite) {
        _gameStateBitmasksRef.setWhitePiecesBit(fromIndex);
    } else {
        _gameStateBitmasksRef.setBlackPiecesBit(fromIndex);
    }
}

representation::board::PieceType MoveRetractor::determineMovedPieceType(
    const representation::move::Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    representation::board::PieceType  previouslyMovedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    if (move.isAnyPromo()) {
        previouslyMovedPieceType = wasWhite ?representation::board::PieceType::W_PAWN :representation::board::PieceType::B_PAWN;
    } else {
        previouslyMovedPieceType = _squaresLookupRef.getPieceTypeAtIndex(toIndex);
    }

    return previouslyMovedPieceType;
}

void MoveRetractor::unmakeMove(
    const representation::move::Move& previousMove, 
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
    representation::board::PieceType  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.isAnyCapture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = determineCaptureIndex(previousMove, wasWhite, toIndex);
        representation::board::PieceType  previouslyCapturedPieceType = _searchMemoryRef.getLastCapturedPieceAtDepth(currentDepth);

        placeBackCapturedPieceOnBoard(previousMove.isEpCapture(), captureIndex, toIndex, wasWhite, previouslyCapturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        _squaresLookupRef.setPieceTypeAtIndex(toIndex,representation::board::PieceType::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    if (previousMove.isDoublePawnPush()) {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }

    if (not previousMove.isAnyCapture() && (previouslyMovedPieceType !=representation::board::PieceType::W_PAWN && previouslyMovedPieceType !=representation::board::PieceType::B_PAWN)) {
        _searchMemoryRef.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }

    _gameStateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

} // namespace makemove
} // namespace logic