#pragma once

#include "model/position/piece_type.h"

namespace model {
    class Move;
}

namespace logic {

struct MoveResult {
    model::Piece::Type capturedPieceType;
    model::Piece::Type movedPieceType;

    MoveResult() {
        capturedPieceType = model::Piece::Type::EMPTY;
        movedPieceType = model::Piece::Type::EMPTY;
    }
};

struct MoveUtils {
    
    static model::Piece::Type getPromotionPieceType(int promotionFlag, bool is_w);
    static int determineCaptureIndex(const model::Move& move, bool is_w, int toIndex);

};


} // namespace logic