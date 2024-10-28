#include "ChessEngine/src/engine/search/SearchMemory.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace engine {
namespace search {

SearchMemory::SearchMemory(int maxDepth) : _maxDepth(maxDepth) 
{
    _castlingRights.resize(_maxDepth + 1);
    _lastCapturedPieces.resize(_maxDepth + 1);
    _enPessantTargets.resize(_maxDepth + 1);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth + 1);

    for (int i = 0; i <= _maxDepth; i++) {
        _castlingRights[i] = 0b1111;
        _lastCapturedPieces[i] = model::PieceType::EMPTY;
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
    model::PieceType movedPieceType) 
{
    if (move.isAnyCastle()) {
        removeCastlingRightsForRemainingDepths(
            currentDepth, 
            isWhite ? whiteBoth : blackBoth
        );
    }

    if (movedPieceType == model::PieceType::W_KING || movedPieceType == model::PieceType::B_KING) {
        if (isWhite) {
            if (_castlingRights[currentDepth] & whiteBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, whiteBoth);
        } else {
            if (_castlingRights[currentDepth] & blackBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, blackBoth);
        }
    }

    if (movedPieceType == model::PieceType::W_ROOK || movedPieceType == model::PieceType::B_ROOK) {
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

} // namespace search
} // namespace engine