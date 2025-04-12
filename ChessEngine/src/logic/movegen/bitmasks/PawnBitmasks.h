#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

struct MoveType {
    std::vector<bitmask> diagonal;
    std::vector<bitmask> straight;
};

struct PawnBitmasks {

    static bitmask getCapturePawnMovesBitmask(int ind, bool isWhite);
    static bitmask getStraightPawnMovesBitmask(int ind, bool isWhite);
    static std::vector<bitmask> getAllStraightPawnMoveBitmasks(bool isWhite);
    static std::vector<bitmask> getAllCapturePawnMoveBitmasks(bool isWhite);
    
    static bitmask applyCapturePawnMoves(int ind, bool isWhite);
    static bitmask removeWrapAroundPawnMoves(bitmask pawnMoves, int ind);

};


} // namespace logic