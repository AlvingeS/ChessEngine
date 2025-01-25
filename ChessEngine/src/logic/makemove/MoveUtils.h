#pragma once

#include "ChessEngine/src/model/board/PieceType.h"

namespace model {
    class Move;
}

namespace logic {

model::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex);

} // namespace logic