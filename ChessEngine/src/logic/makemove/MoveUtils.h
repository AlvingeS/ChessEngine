#pragma once

#include "ChessEngine/src/model/board/PieceType.h"

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

model::Piece::Type getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex);

} // namespace logic