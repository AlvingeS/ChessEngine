#include "PawnBitMasks.h"
#include "ChessUtils.h"

namespace bits {
    U64 applyPawnMoves(int ind, bool isWhite) {
        U64 pawnMoves = 0ULL;

        int shiftUp = isWhite ? 8 : -8;
        int shiftUpLeft = isWhite ? 9 : -9;
        int shiftUpRight = isWhite ? 7 : -7;
        int shiftUpTwo = isWhite ? 16 : -16;

        if (ind + shiftUp <= 63 && ind + shiftUp >= 0) pawnMoves |= (1ULL << (ind + shiftUp));
        if (ind + shiftUpLeft <= 63 && ind + shiftUpLeft >= 0) pawnMoves |= (1ULL << (ind + shiftUpLeft));
        if (ind + shiftUpRight <= 63 && ind + shiftUpRight >= 0) pawnMoves |= (1ULL << (ind + shiftUpRight));

        if (isWhite && ind >= 8 && ind <= 15) {
            pawnMoves |= (1ULL << (ind + shiftUpTwo));
        } else if (!isWhite && ind >= 48 && ind <= 55) {
            pawnMoves |= (1ULL << (ind + shiftUpTwo));
        }

        return pawnMoves;
    }

    void removeWrapAroundPawnMoves(U64& pawnMoves, int ind) {
        U64 notAFile = ~getFileMask(7);
        U64 notHFile = ~getFileMask(0);

        switch (fileFromBitIndex(ind)) {
            case 0:
                pawnMoves &= notAFile;
                break;
            case 7:
                pawnMoves &= notHFile;
                break;
        }
    }

    U64 getPawnBitMask(int ind, bool isWhite) {
        U64 pawnMoves = applyPawnMoves(ind, isWhite);
        removeWrapAroundPawnMoves(pawnMoves, ind);
        return pawnMoves;
    }

    std::vector<U64> getAllPawnBitMasks(bool isWhite) {
        std::vector<U64> pawnBitMasks;


        for (int i = 0; i < 64; i++) {
            pawnBitMasks.push_back(getPawnBitMask(i, isWhite));
        }

        return pawnBitMasks;
    }
}