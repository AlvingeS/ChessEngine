#pragma once

#include "Move.h"
#include "PieceType.h"
#include "ChessBoard.h"
#include "ZHasher.h"
#include "ChessEngine/search/SearchMemory.h"

namespace game {
    class MoveMaker {
        public:
            // Constructor
            MoveMaker(ChessBoard& board, search::SearchMemory& searchMemory, ZHasher& zHasher);
            
            // Public member functions
            void makeMove(Move move, bool isWhite, int currentDepth);
            void unmakeMove(Move move, bool wasWhite, int currentDepth);
            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide);

            search::SearchMemory& getSearchMemory() {
                return _searchMemory;
            }

        private:
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            PieceType getPromotionPieceType(int promotionFlag, bool isWhite);

            ChessBoard& _board;
            search::SearchMemory& _searchMemory;
            ZHasher& _zHasher;
    };
}