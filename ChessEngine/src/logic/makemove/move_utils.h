#pragma once

#include "model/position/piece_type.h"

namespace model {
    class Move;
}

namespace logic {

struct MoveResult {
    model::Piece::Type captured_piece_type;
    model::Piece::Type moved_piece_type;

    MoveResult() {
        captured_piece_type = model::Piece::Type::EMPTY;
        moved_piece_type = model::Piece::Type::EMPTY;
    }
};

struct MoveUtils {
    
    static model::Piece::Type get_promotion_piece_type(int promotionFlag, bool is_w);
    static int determine_capture_sq_idx(const model::Move& move, bool is_w, int to_index);

};


} // namespace logic