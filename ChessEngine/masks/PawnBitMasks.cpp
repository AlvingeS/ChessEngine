#include "PawnBitMasks.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace masks {
U64 applyCapturePawnMoves(const int ind, const bool isWhite) 
{
    U64 captureMoves = 0ULL;

    int shiftUpLeft = isWhite ? 9 : -9;
    int shiftUpRight = isWhite ? 7 : -7;

    if (ind + shiftUpLeft <= 63 && ind + shiftUpLeft >= 0) captureMoves |= (1ULL << (ind + shiftUpLeft));
    if (ind + shiftUpRight <= 63 && ind + shiftUpRight >= 0) captureMoves |= (1ULL << (ind + shiftUpRight));

    return captureMoves;
}

void removeWrapAroundPawnMoves(U64& captureMoves, const int ind) 
{
    U64 notAFile = ~utils::getFileMask(7);
    U64 notHFile = ~utils::getFileMask(0);

    switch (utils::fileFromBitIndex(ind)) {
        case 0:
            captureMoves &= notAFile;
            break;
        case 7:
            captureMoves &= notHFile;
            break;
    }
}

U64 getCapturePawnMovesBitmask(const int ind, const bool isWhite) 
{
    U64 captureMoves = applyCapturePawnMoves(ind, isWhite);
    removeWrapAroundPawnMoves(captureMoves, ind);
    return captureMoves;
}

U64 getStraightPawnMovesBitmask(const int ind, const bool isWhite) 
{
    U64 straightMoves = 0ULL;

    int shiftUp = isWhite ? 8 : -8;
    int shiftUpTwice = isWhite ? 16 : -16;

    if (ind + shiftUp <= 63 && ind + shiftUp >= 0) straightMoves |= (1ULL << (ind + shiftUp));
    if (isWhite && (ind >= 8 && ind <= 15)) {
        if (ind + shiftUpTwice <= 63 && ind + shiftUpTwice >= 0) straightMoves |= (1ULL << (ind + shiftUpTwice));
    } else if (!isWhite && (ind >= 48 && ind <= 55)) {
        if (ind + shiftUpTwice <= 63 && ind + shiftUpTwice >= 0) straightMoves |= (1ULL << (ind + shiftUpTwice));
    }
    return straightMoves;
}

const std::vector<U64> getAllStraightPawnMoveBitmasks(const bool isWhite) 
{
    std::vector<U64> straightPawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        straightPawnMoveBitmasks.push_back(getStraightPawnMovesBitmask(i, isWhite));
    }

    return straightPawnMoveBitmasks;
}

const std::vector<U64> getAllCapturePawnMoveBitmasks(const bool isWhite) 
{
    std::vector<U64> capturePawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        capturePawnMoveBitmasks.push_back(getCapturePawnMovesBitmask(i, isWhite));
    }

    return capturePawnMoveBitmasks;
}

} // namspace masks