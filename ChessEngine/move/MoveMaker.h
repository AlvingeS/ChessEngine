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
    MoveMaker(
        BitBoardUpdater& bitBoardUpdater, 
        BitMaskUpdater& bitMaskUpdater, 
        LookupUpdater& lookupUpdater, 
        perft::SearchMemory& searchMemory, 
        board::ZHasher& zHasher
    );
    
    // Public member functions
    void makeMove(
        const Move move, 
        const bool isWhite, 
        const int currentDepth
    );

    void tryCastling(
        const move::Move &move, 
        const bool isWhite, 
        bool &retFlag
    );

    board::PieceType pickUpPiece(
        const move::Move &move, 
        const bool isWhite, 
        const int fromIndex
    );

    void putDownPiece(
        const move::Move &move, 
        const bool isWhite, int toIndex, 
        const board::PieceType movedPieceType
    );

    void handleCapture(
        const move::Move &move, 
        const bool isWhite, 
        const int toIndex, 
        const int currentDepth
    );

    void handleEnPessantMemory(
        const move::Move &move, 
        const int currentDepth, 
        const bool isWhite, 
        const int toIndex
    );

    void handleNoCaptureCount(
        const move::Move &move, 
        const board::PieceType movedPieceType, 
        const int currentDepth
    );
    
    void unmakeMove(
        const Move move, 
        const bool wasWhite, 
        const int currentDepth
    );

    void tryUncastling(
        const move::Move &move, 
        const bool wasWhite, 
        bool &retFlag
    );

    void handleUncapturing(
        const move::Move &move, 
        const bool wasWhite, 
        const int toIndex, 
        const int currentDepth
    );

    void putBackMovedPiece(
        const int fromIndex, 
        const board::PieceType movedPieceType, 
        const bool wasWhite, 
        const move::Move &move, 
        const int toIndex
    );

    board::PieceType determineMovedPieceType(
        const Move move, 
        const int toIndex, 
        const bool wasWhite
    );
    
    void makeTemporaryKingMove(
        const bool isWhite, 
        const bool isKingSide) 
    {
        _bitBoardUpdater.makeTemporaryKingMove(isWhite, isKingSide);
    }

    void unmakeTemporaryKingMove(
        const bool wasWhite, 
        const bool wasKingSide) 
    {
        _bitBoardUpdater.unmakeTemporaryKingMove(wasWhite, wasKingSide);
    }

    perft::SearchMemory& getSearchMemory() 
    {
        return _searchMemory;
    }

private:

    void makeCastleMove(
        const bool isWhite,
        const bool isKingSide
    );

    void unmakeCastleMove(
        const bool wasWhite, 
        const bool wasKingSide
    );

    board::PieceType getPromotionPieceType(
        const int promotionFlag, 
        const bool isWhite
    );

    BitBoardUpdater _bitBoardUpdater;
    BitMaskUpdater _bitMaskUpdater;
    LookupUpdater _lookupUpdater;
    perft::SearchMemory& _searchMemory;
    board::ZHasher& _zHasher;
};
}