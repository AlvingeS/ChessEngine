#pragma once

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"

namespace model {
    class Board;
    class Move;
    class Bitboards;
    class StateBitmasks;
    class PieceMap;
    class ZHasher;
}

namespace logic {

class MoveMaker {

public:
    MoveMaker(
        model::Board& board,
        engine::SearchMemory& searchMemory
    );
    
    inline engine::SearchMemory& getSearchMemory() const { return _searchMemory; }
    
    void makeMove(
        const model::Move& move,
        bool isWhite, 
        int currentDepth
    );
    
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

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

    model::Bitboards& _bitboards;
    model::StateBitmasks& _stateBitmasks;
    model::PieceMap& _pieceMap;
    model::ZHasher& _zHasher;
    engine::SearchMemory& _searchMemory;
};

} // namespace logic