#pragma once

#include "ChessEngine/board/SquaresLookup.h"

namespace move {
    class LookupUpdater {
        public:
            // ** Constructor **
            LookupUpdater(board::SquaresLookup& squaresLookup);

            // ** Getters & Setters **
            
            // ** Functions **
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);

            board::PieceType getPieceTypeAtIndex(int index) {
                return _squaresLookup.getPieceTypeAtIndex(index);
            }

            void removeCapturedPieceFromLookup(int captureIndex) {
                _squaresLookup.setPieceTypeAtIndex(captureIndex, board::PieceType::EMPTY);
            }

            board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);

            void setPieceTypeAtIndex(int index, board::PieceType pieceType) {
                _squaresLookup.setPieceTypeAtIndex(index, pieceType);
            }

        private:
            // ** Data **
            board::SquaresLookup& _squaresLookup;
        };
}