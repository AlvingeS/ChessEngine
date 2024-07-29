#pragma once

#include "ChessEngine/board/GameStateBitMasks.h"

namespace move {

class BitMaskUpdater {

public:
    // ** Constructor **
    BitMaskUpdater(board::GameStateBitMasks& gameStateBitMasks);

    // ** Getters & Setters **
    
    // ** Functions **
    void makeCastleMove(const bool isWhite, const bool isKingSide);
    void unmakeCastleMove(const bool wasWhite, const bool wasKingSide);

    void setBitForWhitePiecesAtIndex(const int index) {
        _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << index);
    }

    void setBitForBlackPiecesAtIndex(const int index) {
        _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << index);
    }

    void clearBitForWhitePiecesAtIndex(const int index) {
        _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << index);
    }

    void clearBitForBlackPiecesAtIndex(const int index) {
        _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << index);
    }

    void updateOccupiedAndEmptyBitmasks() {
        _gameStateBitMasks.fillOccupiedPiecesBitmask();
        _gameStateBitMasks.fillEmptySquaresBitmask();
    }

private:
    
    // ** Data **
    board::GameStateBitMasks& _gameStateBitMasks;
};

} // namespace move