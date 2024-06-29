#include "KingBitMasks.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace bits {

        // Applies all king moves without considering looping around the board
        U64 applyKingMoves(int ind) {
            U64 kingMoves = 0ULL;
    
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
        void removeWrapAroundKingMoves(U64& kingMoves, int ind) {
            U64 notAFile = ~getFileMask(7);
            U64 notHFile = ~getFileMask(0);
    
            switch (fileFromBitIndex(ind)) {
                case 0:
                    kingMoves &= notAFile;
                    break;
                case 7:
                    kingMoves &= notHFile;
                    break;
            }
        }
    
        U64 getKingBitMask(int ind) {
            U64 kingMoves = applyKingMoves(ind);
            removeWrapAroundKingMoves(kingMoves, ind);
            return kingMoves;
        }
    
        std::vector<U64> getAllKingBitMasks() {
            std::vector<U64> kingBitMasks;
    
            for (int i = 0; i < 64; i++) {
                kingBitMasks.push_back(getKingBitMask(i));
            }
    
            return kingBitMasks;
        }
}