#include "BitBasics.h"
#include <vector>

namespace bits {
    U64 applyPawnMoves(int ind, bool isWhite);
    void removeWrapAroundPawnMoves(U64& pawnMoves, int ind);
    U64 getPawnBitMask(int ind, bool isWhite);
    std::vector<U64> getAllPawnBitMasks(bool isWhite);
}