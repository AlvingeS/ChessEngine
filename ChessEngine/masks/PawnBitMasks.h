#pragma once

#include "ChessEngine/common.h"

namespace masks {

struct MoveType {
    std::vector<U64> diagonal;
    std::vector<U64> straight;
};

U64 applyCapturePawnMoves(const int ind,const  bool isWhite);
void removeWrapAroundPawnMoves(U64& pawnMoves, const int ind);
U64 getCapturePawnMovesBitmask(const int ind, const bool isWhite);
U64 getStraightPawnMovesBitmask(const int ind, const bool isWhite);
const std::vector<U64> getAllStraightPawnMoveBitmasks(const bool isWhite);
const std::vector<U64> getAllCapturePawnMoveBitmasks(const bool isWhite);

} // namespace masks