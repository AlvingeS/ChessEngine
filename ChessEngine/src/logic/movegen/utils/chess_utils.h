#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

namespace logic {
    
struct ChessUtils {
    
    static bitmask getFileMask(int file);
    static bitmask getRankMask(int rank);
    static int rankFromBitIndex(int i);
    static int fileFromBitIndex(int i);
    static bitmask getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces);
    static bitmask getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces);
    static bitmask getWhiteSquaresBitmask();
    static bitmask getBlackSquaresBitmask();
    
    static int abs(int n);
    static int manhattanDistance(int i, int j);

};

} // namespace logic

#include "logic/movegen/utils/chess_utils.inl"