#pragma once

#include "model/move/Movelist.h"

namespace logic {

struct RayLogic {

    static void addMovesFromFreeRay(
        bitmask freeRay,
        int bitIndexFrom,
        model::Movelist& moveList
    );

    static void addMoveIfBlockerIsEnemy(
        int blockerIndex,
        bool isWhite,
        int bitIndexFrom,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask
    );

    static void addMovesBetweenBlockerAndPieceOnStraightRay(
        int blockerIndex,
        bool alongFile, 
        bool startFromBlocker, int rookRank, 
        int rookFile, 
        int bitIndexFrom,
        model::Movelist& moveList
    );

    static void addMovesBetweenBlockerAndPieceOnDiagonalRay(
        int blockerIndex,
        bool startFromBlocker, 
        int bishopRank, 
        int bishopFile, 
        int bitIndexFrom,
        model::Movelist& moveList
    );
    
    static void addMovesFromStraightRay(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static void addMovesFromDiagonalRay(
        bitmask ray,
        bool blockerOnLSB,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static bool checkStraightRay(
        bitmask straightRay,
        bool firstBlockerOnLSB,
        bitmask opponentRooksAndQueens,
        bitmask occupiedPiecesBitmask
    );
    
    static bool checkDiagonalRay(
        bitmask diagonalRay,
        bool firstBlockerOnLSB,
        bitmask opponentBishopsAndQueens,
        bitmask occupiedPiecesBitmask
    );

};

} // namespace logic