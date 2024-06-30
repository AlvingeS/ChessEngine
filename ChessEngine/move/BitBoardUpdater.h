#pragma once

#include "ChessEngine/board/BitBoards.h"

namespace move {
    class BitBoardUpdater {
        public:
            // ** Constructor **
            BitBoardUpdater(board::BitBoards& bitboards);

            // ** Getters & Setters **
            
            // ** Functions **
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            
            void removeBitAtIndexForPieceType(int captureIndex, board::PieceType capturedPieceType) {
                _bitboards.getBitboardFromIndex(board::pieceTypeToInt(capturedPieceType)) &= ~(1ULL << captureIndex);
            }

            void setBitAtIndexForPieceType(int toIndex, board::PieceType promotionPieceType) {
                _bitboards.getBitboardFromIndex(board::pieceTypeToInt(promotionPieceType)) |= (1ULL << toIndex);
            }

            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide);

        private:
            // ** Data **
            board::BitBoards& _bitboards;
        };
}