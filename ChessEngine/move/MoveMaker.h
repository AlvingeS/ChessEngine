#pragma once

#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/board/PieceType.h"

#include "Move.h"

namespace move {

class MoveMaker {

public:
    // Constructor
    MoveMaker(
        board::Bitboards& bitBoards, 
        board::GameStateBitmasks& gameStateBitmasks, 
        board::SquaresLookup& squaresLookup, 
        perft::SearchMemory& searchMemory, 
        board::ZHasher& zHasher
    );
    
    // Public member functions
    void makeMove(
        const Move move, 
        const bool isWhite, 
        const int currentDepth
    );

    board::PieceType pickUpPiece(
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
    
    void makeCastleMove(const bool isWhite, const bool isKingSide);
    void unmakeCastleMove(const bool isWhite, const bool isKingSide);
    void makeTemporaryKingMove(const bool isWhite, const bool isKingSide);
    void unmakeTemporaryKingMove(const bool isWhite, const bool isKingSide);

    perft::SearchMemory& getSearchMemory() 
    {
        return _searchMemory;
    }

private:
    board::PieceType getPromotionPieceType(
        const int promotionFlag, 
        const bool isWhite
    );

    board::Bitboards& _bitboards;
    board::GameStateBitmasks& _gameStateBitmasks;
    board::SquaresLookup& _squaresLookup;
    perft::SearchMemory& _searchMemory;
    board::ZHasher& _zHasher;
};
}