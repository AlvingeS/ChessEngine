#include <iostream>
#include <bitset>
#include "ChessBitTools.h"

namespace bits {
    constexpr U64 FILE_MASK[8] = {
        0x8080808080808080ULL,
        0x4040404040404040ULL,
        0x2020202020202020ULL,
        0x1010101010101010ULL,
        0x0808080808080808ULL,
        0x0404040404040404ULL,
        0x0202020202020202ULL,
        0x0101010101010101ULL
    };
    
    constexpr U64 RANK_MASK[8] = {
        0x00000000000000FFULL,
        0x000000000000FF00ULL,
        0x0000000000FF0000ULL,
        0x00000000FF000000ULL,
        0x000000FF00000000ULL,
        0x0000FF0000000000ULL,
        0x00FF000000000000ULL,
        0xFF00000000000000ULL
    };
    
    U64 getFileMask(int file) {
        return FILE_MASK[file];
    }

    U64 getRankMask(int rank) {
        return RANK_MASK[rank];
    }

    void printU64InBits(unsigned long long int n) {
        std::cout << std::bitset<64>(n) << std::endl;
    }

    std::vector<int> getBitIndices(U64 bitboard) {
        std::vector<int> indices;

        while (bitboard != 0) {
            int index = __builtin_ctzll(bitboard);

            indices.push_back(index);

            bitboard &= (bitboard - 1);
        }

        return indices;
    }
}
