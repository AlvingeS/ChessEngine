#pragma once

#include "ChessEngine/src/model/move/Move.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace logic {
namespace makemove {

model::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex);

} // namespace makemove
} // namespace logic