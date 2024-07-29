#pragma once

#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/move/BitBoardUpdater.h"
#include "ChessEngine/move/BitMaskUpdater.h"
#include "ChessEngine/move/LookupUpdater.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/board/PieceType.h"

#include "Move.h"

namespace move {
    class MoveMaker {
        public:
            // Constructor
            MoveMaker(BitBoardUpdater& bitBoardUpdater, BitMaskUpdater& bitMaskUpdater, LookupUpdater& lookupUpdater, perft::SearchMemory& searchMemory, board::ZHasher& zHasher);
            
            // Public member functions
            void makeMove(Move move, bool isWhite, int currentDepth);

            void tryCastling(move::Move &move, bool isWhite, bool &retFlag);
            board::PieceType pickUpPiece(move::Move &move, bool isWhite, int fromIndex);
            void putDownPiece(move::Move &move, bool isWhite, int toIndex, board::PieceType movedPieceType);
            void handleCapture(move::Move &move, bool isWhite, int toIndex, int currentDepth);
            void handleEnPessantMemory(move::Move &move, int currentDepth, bool isWhite, int toIndex);
            void handleNoCaptureCount(move::Move &move, board::PieceType movedPieceType, int currentDepth);
            
            void unmakeMove(Move move, bool wasWhite, int currentDepth);

            void tryUncastling(move::Move &move, bool wasWhite, bool &retFlag);
            void handleUncapturing(move::Move &move, bool wasWhite, int toIndex, int currentDepth);
            void putBackMovedPiece(int fromIndex, board::PieceType movedPieceType, bool wasWhite, move::Move &move, int toIndex);
            board::PieceType determineMovedPieceType(Move move, int toIndex, bool wasWhite);
           
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