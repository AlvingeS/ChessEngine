#pragma once

#include "model/types.h"
#include "model/move/movelist.h"

#include <optional>

namespace logic::rays {

struct RayCheckDetectionResult {
    std::optional<sq_idx> checker_sq_idx = std::nullopt;
    std::optional<bitmask> check_mask = std::nullopt;
    std::optional<sq_idx> pinned_piece_sq_idx = std::nullopt;
    std::optional<bitmask> pin_ray = std::nullopt;
};

void add_moves_from_free_ray(
    bitmask free_ray,
    sq_idx from_sq,
    model::Movelist& movelist
);

void add_move_if_blocker_is_opp(
    sq_idx blocker_sq,
    bool is_w,
    sq_idx from_sq,
    model::Movelist& movelist,
    bitmask w_pieces_mask
);

void add_moves_between_blocker_and_piece_on_line_ray(
    sq_idx blocker_sq,
    bool alongFile, 
    bool startFromBlocker, int rookRank, 
    int rookFile, 
    sq_idx from_sq,
    model::Movelist& movelist
);

void add_moves_between_blocker_and_pice_on_diag_ray(
    sq_idx blocker_sq,
    bool startFromBlocker, 
    int bishopRank, 
    int bishopFile, 
    sq_idx from_sq,
    model::Movelist& movelist
);

void add_moves_from_line_ray(
    bitmask ray,
    bool blockerOnLSB,
    bool alongFile,
    bool is_w,
    int piece_idx,
    int pieceRank,
    int pieceFile,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_squares_mask
);

void add_moves_from_diag_ray(
    bitmask ray,
    bool blockerOnLSB,
    bool is_w,
    int piece_idx,
    int pieceRank,
    int pieceFile,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_squares_mask
);

RayCheckDetectionResult detect_check_in_line_ray(
    sq_idx king_idx,
    LineDir ray_dir,
    bool first_blocker_on_lsb,
    bitmask opp_rooks_and_queens_mask,
    bitmask opp_pieces_mask,
    bitmask own_pieces_mask
);

RayCheckDetectionResult detect_check_in_diag_ray(
    sq_idx king_idx,
    DiagDir ray_dir,    
    bool first_blocker_on_lsb,
    bitmask opp_bishops_and_queens_mask,
    bitmask opp_pieces_mask,
    bitmask own_pieces_mask
);

bitmask create_diag_attack_mask(sq_idx king_idx, sq_idx blocker_idx, DiagDir ray_dir, bool include_blocker);
bitmask create_line_attack_mask(sq_idx king_idx, sq_idx blocker_idx, LineDir ray_dir, bool include_blocker);

} // namespace logic::rays