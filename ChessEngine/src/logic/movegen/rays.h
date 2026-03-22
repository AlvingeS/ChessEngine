#pragma once

#include "model/types.h"
#include "model/move/movelist.h"

#include <optional>

namespace logic {

struct LegalityInfo;

}

namespace logic::rays {

struct RayCheckDetectionResult {
    std::optional<sq_t> checker_sq = std::nullopt;
    std::optional<bitmask> check_mask = std::nullopt;
    std::optional<sq_t> pinned_piece_sq = std::nullopt;
    std::optional<bitmask> pin_ray = std::nullopt;
};

void add_moves_from_ray(
    Direction dir,
    bool blocker_on_lsb,
    sq_t piece_sq,
    model::Movelist& movelist,
    bitmask occupied_squares_mask,
    bitmask opponent_squares_mask,
    const LegalityInfo& legality_info
);


RayCheckDetectionResult detect_check_in_ray(
    sq_t king_idx,
    Direction dir,    
    bool first_blocker_on_lsb,
    bitmask opp_bishops_and_queens_mask,
    bitmask opp_pieces_mask,
    bitmask own_pieces_mask,
    bool calculate_pins
);

bitmask create_check_response_mask(sq_t king_idx, sq_t blocker_idx, Direction dir, bool include_blocker);

} // namespace logic::rays