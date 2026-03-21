#include "logic/movegen/rays.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"
#include "logic/utils.h"

#include <optional>

namespace logic::rays {

void add_moves_from_ray(
    Direction dir,
    bool blocker_on_lsb,
    sq_idx piece_sq,
    model::Movelist& movelist,
    bitmask occupied_squares_mask,
    bitmask opponent_squares_mask,
    const LegalityInfo& legality_info
    )
{
    bitmask unhindered_ray = attack_tables::rays[piece_sq][dir];
    
    // If the piece is pinned we need to AND it with pin ray
    if (utils::get_bit(legality_info.pinned_mask, piece_sq)) {
        unhindered_ray &= legality_info.pin_rays[piece_sq];
    }

    bitmask blockers_mask = unhindered_ray & occupied_squares_mask;

    // Rays can be divided logically by if the ray has blockers or not, we first handle the free ray case
    if (blockers_mask == 0ULL) {
        if (legality_info.in_check()) {
            bitmask blocking_move_mask = unhindered_ray & legality_info.check_response_mask;

            //  If we can block it is the only legal move
            if (blocking_move_mask != 0ULL) {
                movelist.add_move(model::Move(piece_sq, utils::lsb_idx(blocking_move_mask), model::Move::QUIET_FLAG));
            }

            return;
        }
    
        // If there are no blockers, add all moves from unhindered ray
        utils::for_each_bit(unhindered_ray, [&](sq_idx to_sq) {
            movelist.add_move(model::Move(piece_sq, to_sq, model::Move::QUIET_FLAG));
        });

        return;
    }

    // Now we handle if the blocker mask != 0ULL
    sq_idx first_blocker_sq = blocker_on_lsb ? utils::lsb_idx(blockers_mask) 
                                             : utils::msb_idx(blockers_mask);
    bitmask first_blocker_mask = 0ULL;
    if (blockers_mask != 0ULL) {
        utils::set_bit(first_blocker_mask, first_blocker_sq);
    }
    
    bitmask ray_after_first_blocker = attack_tables::rays[first_blocker_sq][dir];
    bitmask ray_between_piece_and_first_blocker = unhindered_ray & ~ray_after_first_blocker & ~first_blocker_mask;

    // If we are in check we check if there is a possibility to stop it
    if (legality_info.in_check())  {
        bitmask blocking_move_mask = ray_between_piece_and_first_blocker & legality_info.check_response_mask;
        
        //  If we can block it is the only legal move
        if (blocking_move_mask != 0ULL) {
            movelist.add_move(model::Move(piece_sq, utils::lsb_idx(blocking_move_mask), model::Move::QUIET_FLAG));
            return;
        }
        
        // If the first blocker is also the checker, we may capture it and it is the only legal move
        bitmask capture_move_mask = first_blocker_mask & legality_info.check_response_mask;
        if (capture_move_mask != 0ULL) {
            movelist.add_move(model::Move(piece_sq, utils::lsb_idx(capture_move_mask), model::Move::CAPTURE_FLAG));
            return;
        }

        // If we could not capture or block there are no legal moves, return
        return;
    }  else {
        // Add all moves upp until blocker
        utils::for_each_bit(ray_between_piece_and_first_blocker, [&](sq_idx to_sq) {
            movelist.add_move(model::Move(piece_sq, to_sq, model::Move::QUIET_FLAG));
        });
    
        // If the blocker is an opponent and pin-ray has not zeroed it, capture it
        if (utils::get_bit(opponent_squares_mask, first_blocker_sq) && first_blocker_mask != 0ULL) {
            movelist.add_move(model::Move(piece_sq, first_blocker_sq, model::Move::CAPTURE_FLAG));
        }
    }
}

RayCheckDetectionResult detect_check_in_ray(
    sq_idx king_idx,
    Direction dir,
    bool blocker_on_lsb,
    bitmask opp_bishops_and_queens_mask,
    bitmask opp_pieces_mask,
    bitmask own_pieces_mask,
    bool calculate_pins)
{
    RayCheckDetectionResult result;

    bitmask unhindered_ray = attack_tables::rays[king_idx][dir];
    bitmask opp_bishops_and_queens_blocker_mask = unhindered_ray & opp_bishops_and_queens_mask;

    // If no blocker is an opponent bishop or queen, we cannot be in check from diag ray
    if (opp_bishops_and_queens_blocker_mask == 0) {
        return result;
    }

    bitmask blockers_mask = unhindered_ray & (own_pieces_mask | opp_pieces_mask);

    // We know opp_bishops_and_queens_blocker mask != 0ULL, so if there is only
    // one blocker, then it must be an opponent bishop/queen and we must be in check.
    // There cannot be a pinned piece.
    if (utils::pop_count(blockers_mask) == 1) {
        result.checker_sq_idx = utils::lsb_idx(blockers_mask);
        result.check_mask = create_check_response_mask(king_idx, result.checker_sq_idx.value(), dir, true);
        return result;
    }

    // We know now that there are >=2 pieces on the ray.
    // If the first blocker is an opp bishop or queen, we are in check, nothing more to do.
    sq_idx first_blocker_sq = blocker_on_lsb ? utils::lsb_idx(blockers_mask)
                                             : utils::msb_idx(blockers_mask);

    sq_idx first_opp_bishop_or_queen_sq = blocker_on_lsb ? utils::lsb_idx(opp_bishops_and_queens_blocker_mask)
                                                         : utils::msb_idx(opp_bishops_and_queens_blocker_mask);

    if (first_blocker_sq == first_opp_bishop_or_queen_sq) {
        result.checker_sq_idx = first_blocker_sq;
        result.check_mask = create_check_response_mask(king_idx, result.checker_sq_idx.value(), dir, true);
        return result;
    }

    // At this point we have not found a check, and if not interested in pins, we may return.
    if (!calculate_pins) {
        return result;
    }

    // If first blocker is not our own, then it must be a non-bishop/queen piece and is blocking the check
    // Nothing can be pinned.
    bool first_blocker_is_own = utils::get_bit(own_pieces_mask, first_blocker_sq);
    if (!first_blocker_is_own) {
        return result;
    }

    // We know now that the first blocker is our own piece, and that there must be a bishop/queen
    // somewhere behind it. If it is the second piece on the ray, it is pinning our piece.
    bitmask ray_after_first_blocker = attack_tables::rays[first_blocker_sq][dir];
    bitmask blockers_after_first = ray_after_first_blocker & (own_pieces_mask | opp_pieces_mask);

    sq_idx second_blocker_sq = blocker_on_lsb
        ? utils::lsb_idx(blockers_after_first)
        : utils::msb_idx(blockers_after_first);

    if (second_blocker_sq == first_opp_bishop_or_queen_sq) {
        result.pinned_piece_sq_idx = first_blocker_sq;
        result.pin_ray = create_check_response_mask(king_idx, second_blocker_sq, dir, true);
    }

    return result;
}

bitmask create_check_response_mask(sq_idx king_idx, sq_idx blocker_idx, Direction dir, bool include_blocker)
{
    bitmask attack_mask = attack_tables::rays[king_idx][dir] &
                         ~attack_tables::rays[blocker_idx][dir];

    if (!include_blocker) {
        attack_mask &= ~(1ULL << blocker_idx);
    }

    return attack_mask;
}

} // namespace logic
