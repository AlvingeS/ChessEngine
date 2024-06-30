#pragma once

#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/move/BitBoardUpdater.h"
#include "ChessEngine/move/BitMaskUpdater.h"
#include "ChessEngine/move/LookupUpdater.h"
#include "ChessEngine/board/ZHasher.h"

#include "Move.h"

namespace move {
    class MoveMaker {
        public:
            // Constructor
            MoveMaker(BitBoardUpdater& bitBoardUpdater, BitMaskUpdater& bitMaskUpdater, LookupUpdater& lookupUpdater, perft::SearchMemory& searchMemory, board::ZHasher& zHasher);
            
            // Public member functions
            void makeMove(Move move, bool isWhite, int currentDepth);
            void unmakeMove(Move move, bool wasWhite, int currentDepth);

            void makeTemporaryKingMove(bool isWhite, bool isKingSide) {
                _bitBoardUpdater.makeTemporaryKingMove(isWhite, isKingSide);
            }
            
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide) {
                _bitBoardUpdater.unmakeTemporaryKingMove(wasWhite, wasKingSide);
            }

            perft::SearchMemory& getSearchMemory() {
                return _searchMemory;
            }

        private:
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);

            BitBoardUpdater _bitBoardUpdater;
            BitMaskUpdater _bitMaskUpdater;
            LookupUpdater _lookupUpdater;
            perft::SearchMemory& _searchMemory;
            board::ZHasher& _zHasher;
    };
}