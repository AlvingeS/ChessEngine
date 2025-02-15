#pragma once

#include "ChessEngine/src/logic/makemove/MoveUtils.h"

namespace model {
    class Board;
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
        model::Board& board
    );
    
    void unmakeMove(
        const model::Move& move, 
        bool wasWhite, 
        MoveResult previousMoveResults
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

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
};

} // namespace logic