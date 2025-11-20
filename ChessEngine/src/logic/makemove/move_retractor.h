#pragma once

#include "logic/makemove/move_utils.h"

namespace model {
    class Board;
    class Bitboards;
    class OccupancyMasks;
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
    
    void unmake_move(
        const model::Move& move, 
        bool was_w, 
        MoveResult previousMoveResults
    );

    void revert_temporary_king_move(bool was_w, bool is_kside); 

private:
    void unmake_castle_move(bool was_w, bool is_kside);

    model::Piece::Type  determine_moved_piece_type(const model::Move& move, bool was_w, int to_index) const;
    
    void remove_previously_moved_piece_from_board(const model::Move& move, int to_index, model::Piece::Type  previouslyMovedPieceType, bool was_w);
    void place_back_captured_piece_on_board(bool is_ep, int captureIndex, int to_index, bool was_w, model::Piece::Type  previouslyCapturedPieceType);
    void place_back_moved_piece_on_board(bool was_w, int fromIndex, model::Piece::Type  moved_piece_type);

    model::Bitboards& bitboards_;
    model::OccupancyMasks& state_bitmasks_;
    model::PieceMap& piece_map_;
    const model::ZHasher& z_hasher_;
};

} // namespace logic