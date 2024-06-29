#include "PawnBitMasks.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace bits {
    U64 applyCapturePawnMoves(int ind, bool isWhite) {
        U64 captureMoves = 0ULL;

        int shiftUpLeft = isWhite ? 9 : -9;
        int shiftUpRight = isWhite ? 7 : -7;

        if (ind + shiftUpLeft <= 63 && ind + shiftUpLeft >= 0) captureMoves |= (1ULL << (ind + shiftUpLeft));
        if (ind + shiftUpRight <= 63 && ind + shiftUpRight >= 0) captureMoves |= (1ULL << (ind + shiftUpRight));

        return captureMoves;
    }

    void removeWrapAroundPawnMoves(U64& captureMoves, int ind) {
        U64 notAFile = ~getFileMask(7);
        U64 notHFile = ~getFileMask(0);

        switch (fileFromBitIndex(ind)) {
            case 0:
                captureMoves &= notAFile;
                break;
            case 7:
                captureMoves &= notHFile;
                break;
        }
    }

    U64 getCapturePawnMovesBitmask(int ind, bool isWhite) {
        U64 captureMoves = applyCapturePawnMoves(ind, isWhite);
        removeWrapAroundPawnMoves(captureMoves, ind);
        return captureMoves;
    }

    U64 getStraightPawnMovesBitmask(int ind, bool isWhite) {
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

    std::vector<U64> getAllStraightPawnMoveBitmasks(bool isWhite) {
        std::vector<U64> straightPawnMoveBitmasks;

        for (int i = 0; i < 64; i++) {
            straightPawnMoveBitmasks.push_back(getStraightPawnMovesBitmask(i, isWhite));
        }

        return straightPawnMoveBitmasks;
    }

    std::vector<U64> getAllCapturePawnMoveBitmasks(bool isWhite) {
        std::vector<U64> capturePawnMoveBitmasks;

        for (int i = 0; i < 64; i++) {
            capturePawnMoveBitmasks.push_back(getCapturePawnMovesBitmask(i, isWhite));
        }

        return capturePawnMoveBitmasks;
    }
}