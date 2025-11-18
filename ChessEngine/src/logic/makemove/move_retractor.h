#pragma once

#include "logic/makemove/move_utils.h"

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

    void revert_temporary_king_move(bool wasWhite, bool is_kside); 

private:
    void unmakeCastleMove(bool wasWhite, bool is_kside);

    model::Piece::Type  determineMovedPieceType(const model::Move& move, bool wasWhite, int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const model::Move& move, int toIndex, model::Piece::Type  previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite, model::Piece::Type  previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex, model::Piece::Type  movedPieceType);

    model::Bitboards& bitboards_;
    model::StateBitmasks& state_bitmasks_;
    model::PieceMap& piece_map_;
    const model::ZHasher& _zHasher;
};

} // namespace logic