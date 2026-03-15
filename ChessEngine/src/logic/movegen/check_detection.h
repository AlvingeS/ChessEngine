#pragma once

#include "model/types.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/utils.h"

#include <array>

#include <optional>
namespace model {
    class Position;
} // namespace model

namespace logic {

namespace rays {
    struct RayCheckDetectionResult;
} // namespace rays

struct LegalityInfo {
    std::array<std::pair<bool, int>, 8> king_move_offset_response_checklist{{
        {false, 8},   // N
        {false, 9},   // NE
        {false, 1},   // E
        {false, -7},  // SE
        {false, -8},  // S
        {false, -9},  // SW
        {false, -1},  // W
        {false, 7},   // NW
    }};
    bitmask king_blocked_moves_mask = 0ULL;
    bitmask king_response_mask = 0ULL;
    bitmask checkers_mask = 0ULL;
    bitmask check_response_mask = 0ULL;
    bitmask pinned_mask = 0ULL;
    std::array<bitmask, 64> pin_rays{};

    bool in_check() const {return checkers_mask != 0ULL;};
    bool in_double_check() const {return utils::pop_count(checkers_mask) == 2;};
};

class CheckDetection {

public:
    CheckDetection(const model::Position& pos);
    LegalityInfo generate_legality_info() const;
    bool in_check_from_line_rays() const;
    bool in_check() const;
    
private:
    void reverse_raycast(sq_idx king_sq, LegalityInfo& legality_info, bool actual_king_sq, std::optional<bool> is_w_override = std::nullopt) const;
    void handle_ray_check_detection_result(const rays::RayCheckDetectionResult& result, LegalityInfo& legality_info, Direction dir, sq_idx king_sq, bool actual_king_sq) const;
    
    const model::Position& pos_;
};

} // namespace logic