#pragma once

#include "logic/makemove/undo_info.h"
#include "logic/zobrist/z_hasher.h"

#include "model/move/move.h"
#include "model/types.h"

namespace model {
    class Board;
    class Position;
}

namespace logic {

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(model::Position& pos, ZHasher& z_hasher);    
    void unmake_move(const model::Move& move, logic::UndoInfo& undo_info);

private:
    void unmake_castle_move(bool is_kside);
    PieceType  determine_moved_piece_type(const model::Move& move, sq_t to_sq) const;
    void remove_previously_moved_piece_from_board(const model::Move& move, sq_t to_sq, PieceType previous_moved_piece_type);
    void place_back_captured_piece_on_board(sq_t capture_sq, PieceType previous_captured_piece_type);
    void place_back_moved_piece_on_board(sq_t from_sq, PieceType moved_piece_type);

    void restore_state(logic::UndoInfo& undo_info);

    model::Position& pos_;
    logic::ZHasher& z_hasher_;
};

} // namespace logic