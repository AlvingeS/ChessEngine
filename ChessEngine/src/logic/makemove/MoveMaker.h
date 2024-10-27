#pragma once

#include "ChessEngine/src/representation/move/Move.h"

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/PieceType.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"
#include "ChessEngine/src/representation/board/SquaresLookup.h"
#include "ChessEngine/src/representation/board/ZHasher.h"
#include "ChessEngine/src/representation/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"


namespace logic {
namespace makemove {

class MoveMaker {

public:
    // Constructor
    MoveMaker(
        representation::board::Bitboards& bitBoards, 
        representation::board::GameStateBitmasks& gameStateBitmasks, 
        representation::board::SquaresLookup& squaresLookup, 
        representation::board::ZHasher& zHasher,
        engine::search::SearchMemory& searchMemory
    );
    
    // Public member functions
    void makeMove(
        const representation::move::Move& move,
        bool isWhite, 
        int currentDepth
    );

    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

    engine::search::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    void makeCastleMove(bool isWhite, bool isKingSide);
    
    representation::board::PieceType removeMovedPieceFromBoard(
        bool isWhite, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool isWhite, 
        int captureIndex, 
       representation::board::PieceType  capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
       representation::board::PieceType  movedPieceType
    );

    void handleEnPessantMemory(
        const representation::move::Move& move, 
        bool isWhite, 
        int currentDepth, 
        int toIndex
    );

    void handleNoCaptureCount(
        const representation::move::Move& move, 
        int currentDepth,
       representation::board::PieceType  movedPieceType 
    );

    representation::board::Bitboards& _bitboardsRef;
    representation::board::GameStateBitmasks& _gameStateBitmasksRef;
    representation::board::SquaresLookup& _squaresLookupRef;
    const representation::board::ZHasher& _zHasherRef;
    engine::search::SearchMemory& _searchMemoryRef;
};

} // namespace makemove
} // namespace logic