#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"
#include "logic/utils.h"
#include "logic/makemove/undo_info.h"

namespace model {
    class Move;
    class Position;
} // namespace model

namespace logic {

class MoveMaker {

public:
    MoveMaker(model::Position& pos);
    
    UndoInfo make_move(const model::Move& move, bool is_w);
    
    void make_temporary_king_move(bool is_w, bool is_kside);

private:
    void update_ep_target_mask(const model::Move& move, bool is_w);
    void update_castle_rights(const model::Move& move, bool is_w, UndoInfo& undo_info);
    void make_castle_move(bool is_w, bool is_kside);
    void store_state(UndoInfo& undo_info);

    model::Piece::Type remove_moved_piece_from_board(bool is_w, sq_idx from_sq);

    void remove_captured_piece_from_board(
        bool is_ep, 
        bool is_w, 
        sq_idx capture_sq, 
        model::Piece::Type captured_piece_type
    );

    void place_moved_piece_on_board(
        bool is_w, 
        sq_idx to_sq,
        model::Piece::Type moved_piece_type
    );

    model::Position& pos_;
};

} // namespace logic