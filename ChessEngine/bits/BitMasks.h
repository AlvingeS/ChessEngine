#pragma once
#include "BitBasics.h"
#include <vector>

namespace bits {

    struct StraightRays {
        U64 north;
        U64 east;
        U64 south;
        U64 west;
    };
    
    struct DiagonalRays {
        U64 northEast;
        U64 southEast;
        U64 southWest;
        U64 northWest;
    };

    U64 getFileMask(int file);
    U64 getRankMask(int rank);
    U64 getNorthRay(int ind);
    U64 getNorthEastRay(int ind);
    U64 getEastRay(int ind);
    U64 getSouthEastRay(int ind);
    U64 getSouthRay(int ind);
    U64 getSouthWestRay(int ind);
    U64 getWestRay(int ind);
    U64 getNorthWestRay(int ind);
    StraightRays getStraightRays(int ind);
    DiagonalRays getDiagonalRays(int ind);
    std::vector<StraightRays> getAllStraightRayBitMasks();
    std::vector<DiagonalRays> getAllDiagonalRayBitMasks();
    std::vector<std::vector<U64>> getAllKnightAttackBitMasks();
    std::vector<std::vector<U64>> getAllKingAttackBitMasks();
    U64 getWhiteSquaresBitMask();
    U64 getBlackSquaresBitMask();
    U64 getOccupiedSquaresBitMask(U64 whitePieces, U64 blackPieces);
    U64 getEmptySquaresBitmask(U64 whitePieces, U64 blackPieces);
}