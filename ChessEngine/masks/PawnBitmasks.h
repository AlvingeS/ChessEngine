#pragma once

#include "ChessEngine/common.h"

namespace masks {

struct MoveType {
    std::vector<bitmask> diagonal;
    std::vector<bitmask> straight;
};

bitmask applyCapturePawnMoves(const int ind,const  bool isWhite);
void removeWrapAroundPawnMoves(bitmask& pawnMoves, const int ind);
bitmask getCapturePawnMovesBitmask(const int ind, const bool isWhite);
bitmask getStraightPawnMovesBitmask(const int ind, const bool isWhite);
const std::vector<bitmask> getAllStraightPawnMoveBitmasks(const bool isWhite);
const std::vector<bitmask> getAllCapturePawnMoveBitmasks(const bool isWhite);

} // namespace masks