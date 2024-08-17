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
        const Move& move,
        bool isWhite, 
        int currentDepth
    );

    board::PieceType pickUpPiece(bool isWhite, int fromIndex);

    void putDownPiece(
        const move::Move& move, 
        bool isWhite, 
        int toIndex,
        board::PieceType movedPieceType
    );

    void handleCapture(
        const move::Move& move, 
        bool isWhite, 
        int toIndex, 
        int currentDepth
    );

    void handleEnPessantMemory(
        const move::Move& move, 
        bool isWhite, 
        int currentDepth, 
        int toIndex
    );

    void handleNoCaptureCount(
        const move::Move& move, 
        int currentDepth,
        board::PieceType movedPieceType 
    );
    
    void unmakeMove(
        const Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void tryUncastling(
        const move::Move &move, 
        bool wasWhite, 
        bool &retFlag
    );

    void handleUncapturing(
        const move::Move& move, 
        bool wasWhite, 
        int toIndex, 
        int currentDepth
    );

    void putBackMovedPiece(
        const move::Move& move,
        bool wasWhite,
        int fromIndex, 
        int toIndex,
        board::PieceType movedPieceType
    );

    board::PieceType determineMovedPieceType(
        const Move& move, 
        bool wasWhite,
        int toIndex
    ) const;
    
    void makeCastleMove(bool isWhite, bool isKingSide);
    void unmakeCastleMove(bool isWhite, bool isKingSide);
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);

    perft::SearchMemory& getSearchMemory() const
    {
        return _searchMemory;
    }

private:
    board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) const;

    board::Bitboards& _bitboards;
    board::GameStateBitmasks& _gameStateBitmasks;
    board::SquaresLookup& _squaresLookup;
    perft::SearchMemory& _searchMemory;
    board::ZHasher& _zHasher;
};
}