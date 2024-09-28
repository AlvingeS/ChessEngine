#include "ChessEngine/src/game/masks/KnightBitmasks.h"

#include "ChessEngine/src/utils/ChessUtils.h"

namespace masks {

// Applies all knight moves without considering looping around the board
bitmask applyKnightMoves(int ind) 
{
    bitmask knightMoves = 0ULL;

    if (ind + 15 <= 63) knightMoves |= (1ULL << (ind + 15));
    if (ind + 6 <= 63) knightMoves |= (1ULL << (ind + 6));
    if (ind - 10 >= 0) knightMoves |= (1ULL << (ind - 10));
    if (ind - 17 >= 0) knightMoves |= (1ULL << (ind - 17));
    if (ind - 15 >= 0) knightMoves |= (1ULL << (ind - 15));
    if (ind - 6 >= 0) knightMoves |= (1ULL << (ind - 6));
    if (ind + 10 <= 63) knightMoves |= (1ULL << (ind + 10));
    if (ind + 17 <= 63) knightMoves |= (1ULL << (ind + 17));

    return knightMoves;
}

// Applies file masks to knight moves to prevent looping around the board
bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind) 
{
    bitmask aFile = utils::getFileMask(7);
    bitmask bFile = utils::getFileMask(6);
    bitmask notAorBFile = ~(aFile | bFile);

    bitmask gFile = utils::getFileMask(1);
    bitmask hFile = utils::getFileMask(0);
    bitmask notGorHFile = ~(gFile | hFile);

    switch (utils::fileFromBitIndex(ind)) {
        case 0:
            knightMoves &= notAorBFile;
            break;
        case 1:
            knightMoves &= notAorBFile;
            break;
        case 6:
            knightMoves &= notGorHFile;
            break;
        case 7:
            knightMoves &= notGorHFile;
            break;
    }

    return knightMoves;
}

bitmask getKnightBitmask(int ind) 
{
    bitmask knightMoves = applyKnightMoves(ind);
    knightMoves = removeWrapAroundKnightMoves(knightMoves, ind);
    return knightMoves;
}

const std::vector<bitmask> getAllKnightBitmasks() 
{
    std::vector<bitmask> knightBitmasks;

    for (int i = 0; i < 64; i++) {
        knightBitmasks.push_back(getKnightBitmask(i));
    }

    return knightBitmasks;
}

} // namespace masks