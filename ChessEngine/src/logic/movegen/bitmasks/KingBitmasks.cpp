#include "ChessEngine/src/logic/movegen/bitmasks/KingBitmasks.h"

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

// Applies all king moves without considering looping around the board
bitmask applyKingMoves(int ind) 
{
    bitmask kingMoves = 0ULL;

    if (ind + 8 <= 63) kingMoves |= (1ULL << (ind + 8));
    if (ind + 1 <= 63) kingMoves |= (1ULL << (ind + 1));
    if (ind - 8 >= 0) kingMoves |= (1ULL << (ind - 8));
    if (ind - 1 >= 0) kingMoves |= (1ULL << (ind - 1));
    if (ind + 7 <= 63) kingMoves |= (1ULL << (ind + 7));
    if (ind + 9 <= 63) kingMoves |= (1ULL << (ind + 9));
    if (ind - 7 >= 0) kingMoves |= (1ULL << (ind - 7));
    if (ind - 9 >= 0) kingMoves |= (1ULL << (ind - 9));

    return kingMoves;
}

// Applies file masks to king moves to prevent looping around the board
bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind) 
{
    bitmask notAFile = ~ChessUtils::getFileMask(7);
    bitmask notHFile = ~ChessUtils::getFileMask(0);

    switch (ChessUtils::fileFromBitIndex(ind)) {
        case 0:
            kingMoves &= notAFile;
            break;
        case 7:
            kingMoves &= notHFile;
            break;
    }

    return kingMoves;
}

bitmask getKingBitmask(int ind) 
{
    bitmask kingMoves = applyKingMoves(ind);
    kingMoves = removeWrapAroundKingMoves(kingMoves, ind);
    return kingMoves;
}

const std::vector<bitmask> getAllKingBitmasks() 
{
    std::vector<bitmask> kingBitmasks;

    for (int i = 0; i < 64; i++) {
        kingBitmasks.push_back(getKingBitmask(i));
    }

    return kingBitmasks;
}

} // namespace logic