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
    
    void unmake_move(
        const model::Move& move, 
        bool was_w, 
        logic::UndoInfo& undo_info
    );

    void revert_temporary_king_move(bool was_w, bool is_kside); 

private:
    void unmake_castle_move(bool was_w, bool is_kside);

    model::Piece::Type  determine_moved_piece_type(const model::Move& move, bool was_w, sq_idx to_sq) const;
    
    void remove_previously_moved_piece_from_board(const model::Move& move, sq_idx to_sq, model::Piece::Type  previous_moved_piece_type, bool was_w);
    void place_back_captured_piece_on_board(bool is_ep, sq_idx capture_sq, sq_idx to_sq, bool was_w, model::Piece::Type  previous_captured_piece_type);
    void place_back_moved_piece_on_board(bool was_w, sq_idx from_sq, model::Piece::Type  moved_piece_type);

    void restore_state(logic::UndoInfo& undo_info);

    model::Position& pos_;
    logic::ZHasher& z_hasher_;
};

} // namespace logic