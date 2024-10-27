#pragma once

#include "ChessEngine/src/representation/move/Move.h"

#include "ChessEngine/src/representation/board/PieceType.h"

namespace logic {
namespace makemove {

representation::board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const representation::move::Move& move, bool isWhite, int toIndex);

} // namespace makemove
} // namespace logic