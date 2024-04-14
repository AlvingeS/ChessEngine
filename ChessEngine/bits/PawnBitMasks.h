#pragma once
#include "BitBasics.h"
#include <vector>

namespace bits {

    struct MoveType {
        std::vector<U64> diagonal;
        std::vector<U64> straight;
    };

    U64 applyCapturePawnMoves(int ind, bool isWhite);
    void removeWrapAroundPawnMoves(U64& pawnMoves, int ind);
    U64 getCapturePawnMovesBitmask(int ind, bool isWhite);
    U64 getStraightPawnMovesBitmask(int ind, bool isWhite);
    std::vector<U64> getAllStraightPawnMoveBitmasks(bool isWhite);
    std::vector<U64> getAllCapturePawnMoveBitmasks(bool isWhite);
}