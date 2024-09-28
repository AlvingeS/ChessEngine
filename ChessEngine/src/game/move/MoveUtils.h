#pragma once

#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/game/board/PieceType.h"

namespace game {
namespace move {
namespace moveutils {

board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const Move& move, bool isWhite, int toIndex);


} // namespace utils
} // namespace move
} // namespace game