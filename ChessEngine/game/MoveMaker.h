#pragma once

#include "Move.h"
#include "PieceType.h"
#include "ChessBoard.h"

namespace game {
    class MoveMaker {
        public:
            // Constructor
            MoveMaker(ChessBoard& board);

            // Public member functions
            void makeMove(Move move, bool isWhite);
            void unmakeMove(Move move, bool wasWhite);
            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide);

        private:
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            PieceType getPromotionPieceType(int promotionFlag, bool isWhite);

            ChessBoard& _board;
    };
}