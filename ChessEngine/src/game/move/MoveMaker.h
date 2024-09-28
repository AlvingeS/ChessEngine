#pragma once

#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/PieceType.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/ponder/perft/SearchMemory.h"

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

    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

    perft::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    void makeCastleMove(bool isWhite, bool isKingSide);
    
    board::PieceType removeMovedPieceFromBoard(
        bool isWhite, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool isWhite, 
        int captureIndex, 
        board::PieceType capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
        board::PieceType movedPieceType
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

    board::Bitboards& _bitboardsRef;
    board::GameStateBitmasks& _gameStateBitmasksRef;
    board::SquaresLookup& _squaresLookupRef;
    perft::SearchMemory& _searchMemoryRef;
    const board::ZHasher& _zHasherRef;
};
}