#pragma once

#include "model/types.h"

#include "logic/utils.h"
#include "logic/makemove/undo_info.h"
#include "logic/zobrist/z_hasher.h"

namespace model {
    class Move;
    class Position;
} // namespace model

namespace logic {

class MoveMaker {

public:
    MoveMaker(model::Position& pos, ZHasher& z_hasher);
    UndoInfo make_move(const model::Move& move);
    
private:
    void update_ep_target_mask(const model::Move& move);
    void update_castle_rights(const model::Move& move, UndoInfo& undo_info);
    void make_castle_move(bool is_kside);
    void store_state(UndoInfo& undo_info);

    PieceType remove_moved_piece_from_board(sq_t from_sq);

    void remove_captured_piece_from_board(sq_t capture_sq, PieceType captured_piece_type);

    void place_moved_piece_on_board(
        sq_t to_sq,
        PieceType moved_piece_type
    );

    model::Position& pos_;
    logic::ZHasher& z_hasher_;
};

} // namespace logic