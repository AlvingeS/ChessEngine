#include "KnightBitMasks.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace masks {

// Applies all knight moves without considering looping around the board
U64 applyKnightMoves(const int ind) 
{
    U64 knightMoves = 0ULL;

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
void removeWrapAroundKnightMoves(U64& knightMoves, const int ind) 
{
    U64 aFile = utils::getFileMask(7);
    U64 bFile = utils::getFileMask(6);
    U64 notAorBFile = ~(aFile | bFile);

    U64 gFile = utils::getFileMask(1);
    U64 hFile = utils::getFileMask(0);
    U64 notGorHFile = ~(gFile | hFile);

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
}

U64 getKnightBitMask(const int ind) 
{
    U64 knightMoves = applyKnightMoves(ind);
    removeWrapAroundKnightMoves(knightMoves, ind);
    return knightMoves;
}

const std::vector<U64> getAllKnightBitMasks() 
{
    std::vector<U64> knightBitMasks;

    for (int i = 0; i < 64; i++) {
        knightBitMasks.push_back(getKnightBitMask(i));
    }

    return knightBitMasks;
}

} // namespace masks