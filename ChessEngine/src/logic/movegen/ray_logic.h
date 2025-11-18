#pragma once

#include "model/types.h"
#include "model/move/movelist.h"

namespace logic {

struct RayLogic {

    static void add_moves_from_free_ray(
        bitmask freeRay,
        int bitIndexFrom,
        model::Movelist& movelist
    );

    static void add_move_if_blocker_is_enemy(
        int blockerIndex,
        bool is_w,
        int bitIndexFrom,
        model::Movelist& movelist,
        bitmask whitePiecesBitmask
    );

    static void add_moves_between_blocker_and_piece_on_line_ray(
        int blockerIndex,
        bool alongFile, 
        bool startFromBlocker, int rookRank, 
        int rookFile, 
        int bitIndexFrom,
        model::Movelist& movelist
    );

    static void add_moves_between_blocker_and_pice_on_diag_ray(
        int blockerIndex,
        bool startFromBlocker, 
        int bishopRank, 
        int bishopFile, 
        int bitIndexFrom,
        model::Movelist& movelist
    );
    
    static void add_moves_from_line_ray(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool is_w,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& movelist,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static void add_moves_from_diag_ray(
        bitmask ray,
        bool blockerOnLSB,
        bool is_w,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& movelist,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static bool check_line_ray(
        bitmask straightRay,
        bool firstBlockerOnLSB,
        bitmask opponentRooksAndQueens,
        bitmask occupiedPiecesBitmask
    );
    
    static bool check_diag_ray(
        bitmask diagonalRay,
        bool firstBlockerOnLSB,
        bitmask opponentBishopsAndQueens,
        bitmask occupiedPiecesBitmask
    );

};

} // namespace logic