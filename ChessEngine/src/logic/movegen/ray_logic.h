#pragma once

#include "model/types.h"
#include "model/move/movelist.h"

namespace logic {

struct RayLogic {

    static void add_moves_from_free_ray(
        bitmask free_ray,
        int bit_idx_from,
        model::Movelist& movelist
    );

    static void add_move_if_blocker_is_opp(
        int blocker_idx,
        bool is_w,
        int bit_idx_from,
        model::Movelist& movelist,
        bitmask w_pieces_mask
    );

    static void add_moves_between_blocker_and_piece_on_line_ray(
        int blocker_idx,
        bool alongFile, 
        bool startFromBlocker, int rookRank, 
        int rookFile, 
        int bit_idx_from,
        model::Movelist& movelist
    );

    static void add_moves_between_blocker_and_pice_on_diag_ray(
        int blocker_idx,
        bool startFromBlocker, 
        int bishopRank, 
        int bishopFile, 
        int bit_idx_from,
        model::Movelist& movelist
    );
    
    static void add_moves_from_line_ray(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool is_w,
        int piece_idx,
        int pieceRank,
        int pieceFile,
        model::Movelist& movelist,
        bitmask w_pieces_mask,
        bitmask occupied_sqrs_mask
    );
    
    static void add_moves_from_diag_ray(
        bitmask ray,
        bool blockerOnLSB,
        bool is_w,
        int piece_idx,
        int pieceRank,
        int pieceFile,
        model::Movelist& movelist,
        bitmask w_pieces_mask,
        bitmask occupied_sqrs_mask
    );
    
    static bool check_line_ray(
        bitmask line_ray,
        bool first_blocker_on_lsb,
        bitmask opp_rooks_and_queens_mask,
        bitmask occupied_sqrs_mask
    );
    
    static bool check_diag_ray(
        bitmask diag_ray,
        bool first_blocker_on_lsb,
        bitmask opp_bishops_and_queens_mask,
        bitmask occupied_sqrs_mask
    );

};

} // namespace logic