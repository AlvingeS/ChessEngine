#include "KnightBitMasks.h"
#include "ChessUtils.h"

namespace bits {

    // Applies all knight moves without considering looping around the board
    U64 applyKnightMoves(int ind) {
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
    void removeWrapAroundKnightMoves(U64& knightMoves, int ind) {
        U64 aFile = getFileMask(7);
        U64 bFile = getFileMask(6);
        U64 notAorBFile = ~(aFile | bFile);

        U64 gFile = getFileMask(1);
        U64 hFile = getFileMask(0);
        U64 notGorHFile = ~(gFile | hFile);
    
        switch (fileFromBitIndex(ind)) {
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

    U64 getKnightBitMask(int ind) {
        U64 knightMoves = applyKnightMoves(ind);
        removeWrapAroundKnightMoves(knightMoves, ind);
        return knightMoves;
    }

    std::vector<U64> getAllKnightBitMasks() {
        std::vector<U64> knightBitMasks;

        for (int i = 0; i < 64; i++) {
            knightBitMasks.push_back(getKnightBitMask(i));
        }

        return knightBitMasks;
    }
}