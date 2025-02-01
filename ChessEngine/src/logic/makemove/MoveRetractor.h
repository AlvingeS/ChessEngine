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

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(
        model::Board& board,
        engine::SearchMemory& searchMemory 
    );
    
    void unmakeMove(
        const model::Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

    engine::SearchMemory& getSearchMemory() const
    {
        return _searchMemory;
    }

private:
    void unmakeCastleMove(bool wasWhite, bool isKingSide);

    model::PieceType  determineMovedPieceType(const model::Move& move, bool wasWhite, int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const model::Move& move, int toIndex, model::PieceType  previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite, model::PieceType  previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex, model::PieceType  movedPieceType);

    model::Bitboards& _bitboards;
    model::StateBitmasks& _stateBitmasks;
    model::PieceMap& _pieceMap;
    const model::ZHasher& _zHasher;
    engine::SearchMemory& _searchMemory;
};

} // namespace logic