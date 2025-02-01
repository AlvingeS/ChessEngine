#pragma once

#include "ChessEngine/src/model/board/PieceType.h"

namespace logic {

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

bitmask getFileMask(int file);
bitmask getRankMask(int rank);
int rankFromBitIndex(int i);
int fileFromBitIndex(int i);
bitmask getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces);
bitmask getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces);
bitmask getWhiteSquaresBitmask();
bitmask getBlackSquaresBitmask();
int abs(int n);

int manhattanDistance(int i, int j);

} // namespace logic

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.inl"