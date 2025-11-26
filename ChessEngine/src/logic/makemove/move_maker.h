#pragma once

#include "model/types.h"
#include "logic/utils.h"

namespace model {
    class Board;
    class Move;
    class Bitboards;
    class OccupancyMasks;
    class PieceMap;
    class ZHasher;
}

namespace logic {

class MoveMaker {

public:
    MoveMaker(model::Board& board);
    
    utils::MoveResult make_move(const model::Move& move, bool is_w);
    
    void make_temporary_king_move(bool is_w, bool is_kside);

private:
    void make_castle_move(bool is_w, bool is_kside);
    
    model::Piece::Type remove_moved_piece_from_board(
        bool is_w, 
        sq_idx from_sq
    );

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

    model::Bitboards& bbs_;
    model::OccupancyMasks& occupancy_masks_;
    model::PieceMap& piece_map_;
    model::ZHasher& z_hasher_;
};

} // namespace logic