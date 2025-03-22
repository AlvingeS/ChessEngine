#include "ChessEngine/src/engine/pickmove/SearchMemory.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace engine {
    
SearchMemory::SearchMemory(int maxDepth) : _maxDepth(maxDepth) 
{
    _castlingRights.resize(_maxDepth + 1);
    _lastCapturedPieces.resize(_maxDepth + 1);
    _enPessantTargets.resize(_maxDepth + 1);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth + 1);

    for (int i = 0; i <= _maxDepth; i++) {
        _castlingRights[i] = 0b1111;
        _lastCapturedPieces[i] = model::Piece::Type::EMPTY;
        _enPessantTargets[i] = 0ULL;
        _noCapturedOrPawnMoveCounts[i] = 0;
    }
}

void SearchMemory::removeCastlingRightsForRemainingDepths(
    int currentDepth,
    unsigned char rightsToRemove) 
{
    for (int i = currentDepth + 1; i < _maxDepth; i++) {
        _castlingRights[i] &= ~rightsToRemove;
    }
}

void SearchMemory::restoreCastlingRightsForRemainingDepths(int currentDepth) 
{
    for (int i = currentDepth + 1; i < _maxDepth; i++) {
        _castlingRights[i] = _castlingRights[currentDepth];
    }
}

void SearchMemory::setCastlingRights(
    int currentDepth, 
    const model::Move& move, 
    bool isWhite, 
    model::Piece::Type movedPieceType) 
{
    if (move.isAnyCastle()) {
        removeCastlingRightsForRemainingDepths(
            currentDepth, 
            isWhite ? whiteBoth : blackBoth
        );
    }

    if (movedPieceType == model::Piece::Type::W_KING || movedPieceType == model::Piece::Type::B_KING) {
        if (isWhite) {
            if (_castlingRights[currentDepth] & whiteBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, whiteBoth);
        } else {
            if (_castlingRights[currentDepth] & blackBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, blackBoth);
        }
    }

    if (movedPieceType == model::Piece::Type::W_ROOK || movedPieceType == model::Piece::Type::B_ROOK) {
        if (isWhite) {
            if (move.getBitIndexFrom() == 0) {
                if (_castlingRights[currentDepth] & whiteKingSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, whiteKingSide);
            } else if (move.getBitIndexFrom() == 7) {
                if (_castlingRights[currentDepth] & whiteQueenSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, whiteQueenSide);
            }
        } else {
            if (move.getBitIndexFrom() == 56) {
                if (_castlingRights[currentDepth] & blackKingSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, blackKingSide);
            } else if (move.getBitIndexFrom() == 63) {
                if (_castlingRights[currentDepth] & blackQueenSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, blackQueenSide);
            }
        }
    }
}

void SearchMemory::unsetCastlingRights(int currentDepth) 
{
    if (_castlingRights[currentDepth] != _castlingRights[currentDepth + 1]) {
        restoreCastlingRightsForRemainingDepths(currentDepth);
    }
}

void SearchMemory::overrideCastlingRights(unsigned char rights) 
{
    for (int i = 0; i < _maxDepth; i++) {
        _castlingRights[i] = rights;
    }
}

void SearchMemory::handleNoCaptureCount(
    const model::Move& move, 
    int currentDepth, 
    model::Piece::Type  movedPieceType)
{
    // If the move is a capture, reset the no capture count
    if (move.isAnyCapture()) {
        resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is a pawn move, reset the no capture count
    if (movedPieceType == model::Piece::Type::W_PAWN || movedPieceType == model::Piece::Type::B_PAWN) {
        resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is not a capture or pawn move, increment the no capture count
    incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
}

void SearchMemory::handleEnPessantMemory(
    const model::Move& move, 
    bool isWhite, 
    int currentDepth, 
    int toIndex) 
{
    if (not move.isDoublePawnPush()) {
        setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        return;
    }

    if (move.isDoublePawnPush()) {
        bitmask enPessantTarget = isWhite ? (1ULL << (toIndex - 8)) 
                                          : (1ULL << (toIndex + 8));

        setEnPessantTargetAtDepth(currentDepth + 1, enPessantTarget);
        
        // FIXME: Temporary because I don't know how to implement this haha
        // _zHasher.hashEnPassantFile(toIndex % 8);
    }
}

} // namespace engine