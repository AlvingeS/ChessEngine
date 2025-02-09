#include "ChessEngine/src/logic/makemove/MoveMaker.h"

#include "ChessEngine/src/logic/makemove/MoveUtils.h"

#include "ChessEngine/src/model/board/Board.h"
#include "ChessEngine/src/model/move/Move.h"

namespace logic {

MoveMaker::MoveMaker(
    model::Board& board,
    engine::SearchMemory& searchMemory
) : _bitboards(board.bitboards), 
    _stateBitmasks(board.stateBitmasks), 
    _pieceMap(board.pieceMap),
    _zHasher(board.zHasher),
    _searchMemory(searchMemory)
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

    // Pick up the piece from the from square and get the moved piece type
    model::PieceType movedPieceType = removeMovedPieceFromBoard(isWhite, fromIndex);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.isAnyCapture()) {
        // Calculate index of captured piece, might be EP
        int captureIndex = determineCaptureIndex(move, isWhite, toIndex);
        model::PieceType capturedPieceType = _pieceMap.getPieceTypeAtIndex(captureIndex);
        
        _searchMemory.setLastCapturedPieceAtDepth(currentDepth, capturedPieceType);
        
        removeCapturedPieceFromBoard(move.isEpCapture(), isWhite, captureIndex, capturedPieceType);
    }

    // Update the moved piece type if the move is a promotion    
    if (move.isAnyPromo())
        movedPieceType = getPromotionPieceType(move.getFlag(), isWhite);

    // Place the moved piece on the to square
    placeMovedPieceOnBoard(isWhite, toIndex, movedPieceType);

    // Misc. memory handling
    handleEnPessantMemory(move, isWhite, currentDepth, toIndex);
    handleNoCaptureCount(move, currentDepth, movedPieceType);

    // Update occupied and empty squares bitmasks
    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;
        
        _bitboards.clearWhiteKingBit(fromKingInd);
        _bitboards.setWhiteKingBit(toKingInd);
        _bitboards.clearWhiteRooksBit(fromRookInd);
        _bitboards.setWhiteRooksBit(toRookInd);

        _stateBitmasks.clearWhitePiecesBit(fromKingInd);
        _stateBitmasks.setWhitePiecesBit(toKingInd);
        _stateBitmasks.clearWhitePiecesBit(fromRookInd);
        _stateBitmasks.setWhitePiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::PieceType::W_KING);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::PieceType::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboards.clearBlackKingBit(fromKingInd);
        _bitboards.setBlackKingBit(toKingInd);
        _bitboards.clearBlackRooksBit(fromRookInd);
        _bitboards.setBlackRooksBit(toRookInd);

        _stateBitmasks.clearBlackPiecesBit(fromKingInd);
        _stateBitmasks.setBlackPiecesBit(toKingInd);
        _stateBitmasks.clearBlackPiecesBit(fromRookInd);
        _stateBitmasks.setBlackPiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::PieceType::B_KING);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::PieceType::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::PieceType::B_ROOK);
    }

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58) 
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}


model::PieceType MoveMaker::removeMovedPieceFromBoard(bool isWhite, int fromIndex) 
{
    // Determine the piece type of the piece being moved
    model::PieceType  movedPieceType = _pieceMap.getPieceTypeAtIndex(fromIndex);

    // Update zobrist hash
    _zHasher.hashSquarePieceType(fromIndex, movedPieceType);

    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    _bitboards.clearPieceTypeBit(fromIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(fromIndex, model::PieceType::EMPTY);

    isWhite ? _stateBitmasks.clearWhitePiecesBit(fromIndex) 
            : _stateBitmasks.clearBlackPiecesBit(fromIndex);

    return movedPieceType;
}

void MoveMaker::placeMovedPieceOnBoard(
    bool isWhite, 
    int toIndex, 
    model::PieceType movedPieceType) 
{
    _bitboards.setPieceTypeBit(toIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(toIndex, movedPieceType);

    isWhite ? _stateBitmasks.setWhitePiecesBit(toIndex) 
            : _stateBitmasks.setBlackPiecesBit(toIndex);
}

void MoveMaker::handleNoCaptureCount(
    const model::Move& move, 
    int currentDepth, 
    model::PieceType  movedPieceType)
{
    // If the move is a capture, reset the no capture count
    if (move.isAnyCapture()) {
        _searchMemory.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is a pawn move, reset the no capture count
    if (movedPieceType == model::PieceType::W_PAWN || movedPieceType == model::PieceType::B_PAWN) {
        _searchMemory.resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is not a capture or pawn move, increment the no capture count
    _searchMemory.incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
}

void MoveMaker::handleEnPessantMemory(
    const model::Move& move, 
    bool isWhite, 
    int currentDepth, 
    int toIndex) 
{
    if (not move.isDoublePawnPush()) {
        _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        return;
    }

    if (move.isDoublePawnPush()) {
        bitmask enPessantTarget = isWhite ? (1ULL << (toIndex - 8)) 
                                          : (1ULL << (toIndex + 8));

        _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, enPessantTarget);
    }
}

void MoveMaker::removeCapturedPieceFromBoard(bool isEP, bool isWhite, int captureIndex, model::PieceType  capturedPieceType) {
    // Remove captured piece from board models
    _bitboards.clearPieceTypeBit(captureIndex, capturedPieceType);

    isWhite ? _stateBitmasks.clearBlackPiecesBit(captureIndex) 
            : _stateBitmasks.clearWhitePiecesBit(captureIndex);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture index points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (isEP) {
        _pieceMap.setPieceTypeAtIndex(captureIndex, model::PieceType::EMPTY);
    }
}

} // namespace logic