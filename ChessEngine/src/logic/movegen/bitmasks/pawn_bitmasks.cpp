#include "logic/movegen/bitmasks/pawn_bitmasks.h"

#include "logic/movegen/utils/chess_utils.h"

namespace logic {

bitmask PawnBitmasks::applyCapturePawnMoves(int ind, bool isWhite) 
{
    bitmask captureMoves = 0ULL;

    int shiftUpLeft = isWhite ? 9 : -9;
    int shiftUpRight = isWhite ? 7 : -7;

    if (ind + shiftUpLeft <= 63 && ind + shiftUpLeft >= 0) captureMoves |= (1ULL << (ind + shiftUpLeft));
    if (ind + shiftUpRight <= 63 && ind + shiftUpRight >= 0) captureMoves |= (1ULL << (ind + shiftUpRight));

    return captureMoves;
}

bitmask PawnBitmasks::removeWrapAroundPawnMoves(bitmask captureMoves, int ind) 
{
    bitmask notAFile = ~ChessUtils::getFileMask(7);
    bitmask notHFile = ~ChessUtils::getFileMask(0);

    switch (ChessUtils::fileFromBitIndex(ind)) {
        case 0:
            captureMoves &= notAFile;
            break;
        case 7:
            captureMoves &= notHFile;
            break;
    }

    return captureMoves;
}

bitmask PawnBitmasks::getCapturePawnMovesBitmask(int ind, bool isWhite) 
{
    bitmask captureMoves = applyCapturePawnMoves(ind, isWhite);
    captureMoves = removeWrapAroundPawnMoves(captureMoves, ind);
    return captureMoves;
}

bitmask PawnBitmasks::getStraightPawnMovesBitmask(int ind, bool isWhite) 
{
    bitmask straightMoves = 0ULL;

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

std::vector<bitmask> PawnBitmasks::getAllStraightPawnMoveBitmasks(bool isWhite) 
{
    std::vector<bitmask> straightPawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        straightPawnMoveBitmasks.push_back(getStraightPawnMovesBitmask(i, isWhite));
    }

    return straightPawnMoveBitmasks;
}

std::vector<bitmask> PawnBitmasks::getAllCapturePawnMoveBitmasks(bool isWhite) 
{
    std::vector<bitmask> capturePawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        capturePawnMoveBitmasks.push_back(getCapturePawnMovesBitmask(i, isWhite));
    }

    return capturePawnMoveBitmasks;
}

} // namespace logic