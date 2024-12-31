#pragma once

#include "ChessEngine/src/model/move/Move.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace logic {

model::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex);

} // namespace logic