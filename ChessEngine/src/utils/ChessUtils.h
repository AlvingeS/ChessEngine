#pragma once

#include "ChessEngine/src/utils/maths.h"

#include "ChessEngine/src/game/board/PieceType.h"

namespace utils {

constexpr bitmask FILE_MASK[8] = {
    0x0101010101010101ULL,
    0x0202020202020202ULL,
    0x0404040404040404ULL,
    0x0808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL
};

constexpr bitmask RANK_MASK[8] = {
    0x00000000000000FFULL,
    0x000000000000FF00ULL,
    0x0000000000FF0000ULL,
    0x00000000FF000000ULL,
    0x000000FF00000000ULL,
    0x0000FF0000000000ULL,
    0x00FF000000000000ULL,
    0xFF00000000000000ULL
};

inline bitmask getFileMask(int file) 
{
    return FILE_MASK[file];
}

inline bitmask getRankMask(int rank) 
{
    return RANK_MASK[rank];
}

inline int rankFromBitIndex(int i) 
{
    return i / 8;
}

inline int fileFromBitIndex(int i) 
{
    return i % 8;
}

inline bitmask getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return whitePieces | blackPieces;
}

inline bitmask getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return ~getOccupiedSquaresBitmask(whitePieces, blackPieces);
}

inline bitmask getWhiteSquaresBitmask() 
{
    return 0xAA55AA55AA55AA55ULL;
}

inline bitmask getBlackSquaresBitmask() 
{
    return 0x55AA55AA55AA55AAULL;
}

inline int manhattanDistance(int i, int j) 
{
    int rankDiff = rankFromBitIndex(i) - rankFromBitIndex(j);
    int fileDiff = fileFromBitIndex(i) - fileFromBitIndex(j);
    return abs(rankDiff) + abs(fileDiff);
}

} // namespace utils
