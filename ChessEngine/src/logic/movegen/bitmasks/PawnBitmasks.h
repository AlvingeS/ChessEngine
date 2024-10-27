#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace movegen {
namespace bitmasks {

struct MoveType {
    std::vector<bitmask> diagonal;
    std::vector<bitmask> straight;
};

bitmask applyCapturePawnMoves(int ind, bool isWhite);
bitmask removeWrapAroundPawnMoves(bitmask pawnMoves, int ind);
bitmask getCapturePawnMovesBitmask(int ind, bool isWhite);
bitmask getStraightPawnMovesBitmask(int ind, bool isWhite);
std::vector<bitmask> getAllStraightPawnMoveBitmasks(bool isWhite);
std::vector<bitmask> getAllCapturePawnMoveBitmasks(bool isWhite);

} // namespace bitmasks
} // namespace movegen
} // namespace logic