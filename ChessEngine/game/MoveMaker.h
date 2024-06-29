#pragma once

#include "ChessEngine/perft/SearchMemory.h"

#include "Move.h"
#include "PieceType.h"
#include "ChessBoard.h"
#include "ZHasher.h"

namespace game {
    class MoveMaker {
        public:
            // Constructor
            MoveMaker(ChessBoard& board, perft::SearchMemory& searchMemory, ZHasher& zHasher);
            
            // Public member functions
            void makeMove(Move move, bool isWhite, int currentDepth);
            void unmakeMove(Move move, bool wasWhite, int currentDepth);
            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide);

            perft::SearchMemory& getSearchMemory() {
                return _searchMemory;
            }

        private:
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            PieceType getPromotionPieceType(int promotionFlag, bool isWhite);

            ChessBoard& _board;
            perft::SearchMemory& _searchMemory;
            ZHasher& _zHasher;
    };
}