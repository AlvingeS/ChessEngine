#include "ChessEngine/src/logic/makemove/MoveMaker.h"

#include "ChessEngine/src/logic/makemove/MoveUtils.h"

#include "ChessEngine/src/model/move/Move.h"

namespace logic {

MoveMaker::MoveMaker(
    model::Bitboards& bitBoards, 
    model::StateBitmasks& stateBitmasks, 
    model::PieceMap& pieceMap, 
    model::ZHasher& zHasher,
    engine::SearchMemory& searchMemory
) : _bitboardsRef(bitBoards), 
    _stateBitmasksRef(stateBitmasks), 
    _pieceMapRef(pieceMap), 
    _searchMemoryRef(searchMemory), 
    _zHasherRef(zHasher)
{}

void MoveMaker::makeMove(
    const model::Move& move, 
    bool isWhite, 
    int currentDepth) 
{
    // If the move is a castle, update and return
    if (move.isAnyCastle()) {
        makeCastleMove(isWhite, move.isKingCastle());

        return;
    }

    // Get the from and to indices
    int fromIndex = move.getBitIndexFrom();
    int toIndex = move.getBitIndexTo();
    //assert(fromIndex != toIndex);

    // Pick up the piece from the from square and get the moved piece type
    model::PieceType  movedPieceType = removeMovedPieceFromBoard(isWhite, fromIndex);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.isAnyCapture()) {
        // Calculate index of captured piece, might be EP
        int captureIndex = determineCaptureIndex(move, isWhite, toIndex);
        model::PieceType  capturedPieceType = _pieceMapRef.getPieceTypeAtIndex(captureIndex);
        
        _searchMemoryRef.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);
        
        removeCapturedPieceFromBoard(move.isEpCapture(), isWhite, captureIndex, capturedPieceType);
    }

    // Update the moved piece type if the move is a promotion    
    if (move.isAnyPromo()) {
        movedPieceType = getPromotionPieceType(move.getFlag(), isWhite);
    }

    // Place the moved piece on the to square
    placeMovedPieceOnBoard(isWhite, toIndex, movedPieceType);

    // Misc. memory handling
    handleEnPessantMemory(move, isWhite, currentDepth, toIndex);
    handleNoCaptureCount(move, currentDepth, movedPieceType);

    // Update occupied and empty squares bitmasks
    _stateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;
        
        _bitboardsRef.clearWhiteKingBit(fromKingInd);
        _bitboardsRef.setWhiteKingBit(toKingInd);
        _bitboardsRef.clearWhiteRooksBit(fromRookInd);
        _bitboardsRef.setWhiteRooksBit(toRookInd);

        _stateBitmasksRef.clearWhitePiecesBit(fromKingInd);
        _stateBitmasksRef.setWhitePiecesBit(toKingInd);
        _stateBitmasksRef.clearWhitePiecesBit(fromRookInd);
        _stateBitmasksRef.setWhitePiecesBit(toRookInd);

        _pieceMapRef.setPieceTypeAtIndex(fromKingInd, model::PieceType::EMPTY);
        _pieceMapRef.setPieceTypeAtIndex(toKingInd, model::PieceType::W_KING);
        _pieceMapRef.setPieceTypeAtIndex(fromRookInd, model::PieceType::EMPTY);
        _pieceMapRef.setPieceTypeAtIndex(toRookInd, model::PieceType::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboardsRef.clearBlackKingBit(fromKingInd);
        _bitboardsRef.setBlackKingBit(toKingInd);
        _bitboardsRef.clearBlackRooksBit(fromRookInd);
        _bitboardsRef.setBlackRooksBit(toRookInd);

        _stateBitmasksRef.clearBlackPiecesBit(fromKingInd);
        _stateBitmasksRef.setBlackPiecesBit(toKingInd);
        _stateBitmasksRef.clearBlackPiecesBit(fromRookInd);
        _stateBitmasksRef.setBlackPiecesBit(toRookInd);

        _pieceMapRef.setPieceTypeAtIndex(fromKingInd, model::PieceType::EMPTY);
        _pieceMapRef.setPieceTypeAtIndex(toKingInd, model::PieceType::B_KING);
        _pieceMapRef.setPieceTypeAtIndex(fromRookInd, model::PieceType::EMPTY);
        _pieceMapRef.setPieceTypeAtIndex(toRookInd, model::PieceType::B_ROOK);
    }

    _stateBitmasksRef.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58) 
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboardsRef.clearWhiteKingBit(from);
        _bitboardsRef.setWhiteKingBit(to);
    } else {
        _bitboardsRef.clearBlackKingBit(from);
        _bitboardsRef.setBlackKingBit(to);
    }
}


model::PieceType MoveMaker::removeMovedPieceFromBoard(bool isWhite, int fromIndex) 
{
    // Determine the piece type of the piece being moved
   model::PieceType  movedPieceType = _pieceMapRef.getPieceTypeAtIndex(fromIndex);
    //assert(movedPieceType !=model::PieceType::EMPTY);
    
    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    _pieceMapRef.setPieceTypeAtIndex(fromIndex, model::PieceType::EMPTY);
    _bitboardsRef.clearPieceTypeBit(fromIndex, movedPieceType);

    if (isWhite) {
        _stateBitmasksRef.clearWhitePiecesBit(fromIndex);
    } else {
        _stateBitmasksRef.clearBlackPiecesBit(fromIndex);
    }

    return movedPieceType;
}

void MoveMaker::placeMovedPieceOnBoard(
    bool isWhite, 
    int toIndex, 
   model::PieceType  movedPieceType) 
{
    _bitboardsRef.setPieceTypeBit(toIndex, movedPieceType);
    _pieceMapRef.setPieceTypeAtIndex(toIndex, movedPieceType);

    if (isWhite) {
        _stateBitmasksRef.setWhitePiecesBit(toIndex);
    } else {
        _stateBitmasksRef.setBlackPiecesBit(toIndex);
    }
}

void MoveMaker::handleNoCaptureCount(
    const model::Move& move, 
    int currentDepth, 
   model::PieceType  movedPieceType)
{
    if (not move.isAnyCapture() && (movedPieceType !=model::PieceType::W_PAWN && movedPieceType !=model::PieceType::B_PAWN)) {
        _searchMemoryRef.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    } else {
        _searchMemoryRef.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
    }
}

void MoveMaker::handleEnPessantMemory(
    const model::Move& move, 
    bool isWhite, 
    int currentDepth, 
    int toIndex) 
{
    if (move.isDoublePawnPush()) {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, isWhite ? (1ULL << (toIndex - 8)) : (1ULL << (toIndex + 8)));
    } else {
        _searchMemoryRef.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
    }
}

void MoveMaker::removeCapturedPieceFromBoard(bool isEP, bool isWhite, int captureIndex, model::PieceType  capturedPieceType) {
    // Remove captured piece from board models
    _bitboardsRef.clearPieceTypeBit(captureIndex, capturedPieceType);

    if (isWhite) {
        _stateBitmasksRef.clearBlackPiecesBit(captureIndex);
    } else {
        _stateBitmasksRef.clearWhitePiecesBit(captureIndex);
    }

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture index points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (isEP) {
        _pieceMapRef.setPieceTypeAtIndex(captureIndex, model::PieceType::EMPTY);
    }
}

} // namespace logic