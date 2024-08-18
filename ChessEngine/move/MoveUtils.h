#pragma once

#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/move/Move.h"

namespace move {
namespace moveutils {

board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
int determineCaptureIndex(const Move& move, bool isWhite, int toIndex);


} // namespace utils
} // namespace move