#pragma once

#include "logic/makemove/move_utils.h"

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
    MoveMaker(model::Board& board);
    
    MoveResult makeMove(const model::Move& move, bool is_w);
    
    void make_temporary_king_move(bool is_w, bool is_kside);

private:
    void makeCastleMove(bool is_w, bool is_kside);
    
    model::Piece::Type removeMovedPieceFromBoard(
        bool is_w, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool is_w, 
        int captureIndex, 
        model::Piece::Type capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool is_w, 
        int toIndex,
        model::Piece::Type movedPieceType
    );

    model::Bitboards& bitboards_;
    model::StateBitmasks& state_bitmasks_;
    model::PieceMap& piece_map_;
    model::ZHasher& _zHasher;
};

} // namespace logic