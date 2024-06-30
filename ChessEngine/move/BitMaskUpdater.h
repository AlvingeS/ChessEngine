#pragma once

#include "ChessEngine/board/GameStateBitMasks.h"

namespace move {
    class BitMaskUpdater {
        public:
            // ** Constructor **
            BitMaskUpdater(board::GameStateBitMasks& gameStateBitMasks);

            // ** Getters & Setters **
            
            // ** Functions **
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);

            void setBitForWhitePiecesAtIndex(int index) {
                _gameStateBitMasks.getWhitePiecesBitmask() |= (1ULL << index);
            }

            void setBitForBlackPiecesAtIndex(int index) {
                _gameStateBitMasks.getBlackPiecesBitmask() |= (1ULL << index);
            }

            void clearBitForWhitePiecesAtIndex(int index) {
                _gameStateBitMasks.getWhitePiecesBitmask() &= ~(1ULL << index);
            }

            void clearBitForBlackPiecesAtIndex(int index) {
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
}