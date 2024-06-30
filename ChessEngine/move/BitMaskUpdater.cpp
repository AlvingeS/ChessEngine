#include "BitMaskUpdater.h"

namespace move {
    BitMaskUpdater::BitMaskUpdater(board::GameStateBitMasks& gameStateBitMasks) : _gameStateBitMasks(gameStateBitMasks) {}

    void BitMaskUpdater::makeCastleMove(bool isWhite, bool isKingSide) {
        if (isWhite) {
            if (isKingSide) {
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 1);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 2);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 3);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 0);
            } else {
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 5);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 4);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 3);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 7);
            }
        } else {
            if (isKingSide) {
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 57);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 58);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 59);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 56);
            } else {
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 61);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 60);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 59);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 63);
            }
        }

        _gameStateBitMasks.fillOccupiedPiecesBitmask();
        _gameStateBitMasks.fillEmptySquaresBitmask();
    }

    void BitMaskUpdater::unmakeCastleMove(bool wasWhite, bool wasKingSide) {
        if (wasWhite) {
            if (wasKingSide) {
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 1);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 2);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 3);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 0);
            } else {
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 5);
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << 4);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 3);
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << 7);
            }
        } else {
            if (wasKingSide) {
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 57);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 58);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 59);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 56);
            } else {
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 61);
                _gameStateBitMasks.getBlackPiecesBitmask() &= ~(1ULL << 60);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 59);
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << 63);
            }
        }

        _gameStateBitMasks.fillOccupiedPiecesBitmask();
        _gameStateBitMasks.fillEmptySquaresBitmask();
    }
}