#pragma once

#include "ChessEngine/src/model/move/Move.h"

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/PieceType.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"
#include "ChessEngine/src/model/board/PieceMap.h"
#include "ChessEngine/src/model/board/ZHasher.h"
#include "ChessEngine/src/model/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"


namespace logic {

class MoveMaker {

public:
    // Constructor
    MoveMaker(
        model::Bitboards& bitBoards, 
        model::StateBitmasks& stateBitmasks, 
        model::PieceMap& pieceMap, 
        model::ZHasher& zHasher,
        engine::SearchMemory& searchMemory
    );
    
    // Public member functions
    void makeMove(
        const model::Move& move,
        bool isWhite, 
        int currentDepth
    );

    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

    engine::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    void makeCastleMove(bool isWhite, bool isKingSide);
    
    model::PieceType removeMovedPieceFromBoard(
        bool isWhite, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool isWhite, 
        int captureIndex, 
       model::PieceType  capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
       model::PieceType  movedPieceType
    );

    void handleEnPessantMemory(
        const model::Move& move, 
        bool isWhite, 
        int currentDepth, 
        int toIndex
    );

    void handleNoCaptureCount(
        const model::Move& move, 
        int currentDepth,
       model::PieceType  movedPieceType 
    );

    model::Bitboards& _bitboardsRef;
    model::StateBitmasks& _stateBitmasksRef;
    model::PieceMap& _pieceMapRef;
    const model::ZHasher& _zHasherRef;
    engine::SearchMemory& _searchMemoryRef;
};

} // namespace logic