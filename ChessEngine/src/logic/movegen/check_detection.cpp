#include "logic/movegen/check_detection.h"

#include "model/position/position.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/rays.h"

#include <stdexcept>

namespace {

Direction offset_to_dir(int offset) {
    // The knight may only block a square diagonal to the king when checking the king
    switch (offset) {
        case  9: {  return Direction::NE; };
        case -7: {  return Direction::SE; };
        case -9: {  return Direction::SW; };
        case  7: {  return Direction::NW; };
        default:
            throw "Invalid offset";
    }
}

Direction opposite_direction(Direction dir) {
    switch (dir) {
        case Direction::N:  {return Direction::S;};
        case Direction::NE: {return Direction::SW;};
        case Direction::E:  {return Direction::W;};
        case Direction::SE: {return Direction::NW;};
        case Direction::S:  {return Direction::N;};
        case Direction::SW: {return Direction::NE;};
        case Direction::W:  {return Direction::E;};
        case Direction::NW: {return Direction::SE;};
        default:
            throw "No such direction";
    }
}



// int one_square_away_in_opposite_of_dir_offset(Direction dir) {
//     switch (dir) {
//         case Direction::N:  {return -8;};
//         case Direction::NE: {return -9;};
//         case Direction::E:  {return -1;};
//         case Direction::SE: {return 7;};
//         case Direction::S:  {return 8;};
//         case Direction::SW: {return -9;};
//         case Direction::W:  {return 1;};
//         case Direction::NW: {return -7;};
//         default:
//             throw "No such direction";
//     }
// }

}

namespace logic {

CheckDetection::CheckDetection(const model::Position& pos)
    : pos_(pos)   
{}

LegalityInfo CheckDetection::generate_legality_info() const
{
    LegalityInfo legality_info{};

    sq_idx king_sq = utils::lsb_idx(pos_.is_w ? pos_.bbs.get_w_king_bb()
                                              : pos_.bbs.get_b_king_bb());


    reverse_raycast(king_sq, legality_info, true);
    bitmask own_pieces = pos_.is_w ? pos_.occ_masks.get_w_pieces_mask()
                                   : pos_.occ_masks.get_b_pieces_mask();

    for (const auto& pair : legality_info.king_move_offset_response_checklist) {
        if (pair.first) { 
            // This move has already been deemed not possible
            continue;
        }

        if (king_sq + pair.second < 0) {
            continue;
        }

        if (king_sq + pair.second > 63) {
            continue;
        }

        // If our own piece is blocking the target square, it is blocked
        if (utils::get_bit(own_pieces, king_sq + pair.second)) {
            utils::set_bit(legality_info.king_blocked_moves_mask, king_sq + pair.second);
            continue;
        }

        reverse_raycast(king_sq + pair.second, legality_info, false);
    }

    // And-gate the possible moves with the non-blocked moves
    legality_info.king_response_mask = attack_tables::king[king_sq] & ~legality_info.king_blocked_moves_mask;

    return legality_info;
}

void CheckDetection::handle_ray_check_detection_result(const rays::RayCheckDetectionResult& result, LegalityInfo& legality_info, Direction dir, sq_idx king_sq, bool actual_king_sq) const
{
    if (result.checker_sq_idx.has_value()) {        
        if (actual_king_sq) {
            // We are in check, set the position of the checker and update check response mask
            utils::set_bit(legality_info.checkers_mask, result.checker_sq_idx.value());
            legality_info.check_response_mask = result.check_mask.value();

            // Mark that the direction of the check is a blocked movement for the king
            // if not within one square
            if ((attack_tables::king[king_sq] & legality_info.check_response_mask) == 0ULL) {
                int move_offset = legality_info.king_move_offset_response_checklist[dir].second;

                if (king_sq + move_offset >= 0 && king_sq + move_offset <= 63) {
                    utils::set_bit(legality_info.king_blocked_moves_mask, king_sq + move_offset);
                }

                legality_info.king_move_offset_response_checklist[dir].first = true;
            }

            // Mark that the opposite direction of the check is a blocked movement for the king
            Direction opposite_dir = opposite_direction(dir);
            int opposite_move_offset = legality_info.king_move_offset_response_checklist[opposite_dir].second;
            
            if (king_sq + opposite_move_offset >= 0 && king_sq + opposite_move_offset <= 63) {
                utils::set_bit(legality_info.king_blocked_moves_mask, king_sq + opposite_move_offset);
            }
            
            legality_info.king_move_offset_response_checklist[opposite_dir].first = true;
        } else {
            // The king is in a temporary square and in check, blocked move
            utils::set_bit(legality_info.king_blocked_moves_mask, king_sq);
        }

        return;
    }

    if (result.pinned_piece_sq_idx.has_value()) {
        // Set which square has a pinned piece, and add the pin pask for that square
        utils::set_bit(legality_info.pinned_mask, result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[result.pinned_piece_sq_idx.value()] = result.pin_ray.value();
    }   
}

void CheckDetection::reverse_raycast(sq_idx king_sq, LegalityInfo& legality_info, bool actual_king_sq, std::optional<bool> is_w_override) const
{
    bool is_w = is_w_override.value_or(pos_.is_w);

    bitmask opp_pieces = is_w ? pos_.occ_masks.get_b_pieces_mask()
                              : pos_.occ_masks.get_w_pieces_mask();
    bitmask own_pieces = is_w ? pos_.occ_masks.get_w_pieces_mask()
                              : pos_.occ_masks.get_b_pieces_mask();

    // Check if any opponent rooks or queens are attacking the king
    bitmask opp_rooks_and_queens_mask = is_w ? pos_.bbs.get_b_rooks_bb() | pos_.bbs.get_b_queens_bb()
                                             : pos_.bbs.get_w_rooks_bb() | pos_.bbs.get_w_queens_bb();

    rays::RayCheckDetectionResult n_ray_result = rays::detect_check_in_ray(king_sq, Direction::N, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);

    handle_ray_check_detection_result(n_ray_result, legality_info, N, king_sq, actual_king_sq);

    rays::RayCheckDetectionResult e_ray_result = rays::detect_check_in_ray(king_sq, Direction::E, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(e_ray_result, legality_info, E, king_sq, actual_king_sq);


    rays::RayCheckDetectionResult s_ray_result = rays::detect_check_in_ray(king_sq, Direction::S, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(s_ray_result, legality_info, S, king_sq, actual_king_sq);


    rays::RayCheckDetectionResult w_ray_result = rays::detect_check_in_ray(king_sq, Direction::W, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(w_ray_result, legality_info, W, king_sq, actual_king_sq);

    // Check if any opponent bishops or queens are attacking the king
    bitmask opp_bishops_and_queens_mask = is_w ? pos_.bbs.get_b_bishops_bb() | pos_.bbs.get_b_queens_bb() 
                                               : pos_.bbs.get_w_bishops_bb() | pos_.bbs.get_w_queens_bb();

    rays::RayCheckDetectionResult ne_ray_result = rays::detect_check_in_ray(king_sq, Direction::NE, true, opp_bishops_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(ne_ray_result, legality_info, NE, king_sq, actual_king_sq);


    rays::RayCheckDetectionResult se_ray_result = rays::detect_check_in_ray(king_sq, Direction::SE, false, opp_bishops_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(se_ray_result, legality_info, SE, king_sq, actual_king_sq);


    rays::RayCheckDetectionResult sw_ray_result = rays::detect_check_in_ray(king_sq, Direction::SW, false, opp_bishops_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(sw_ray_result, legality_info, SW, king_sq, actual_king_sq);


    rays::RayCheckDetectionResult nw_ray_result = rays::detect_check_in_ray(king_sq, Direction::NW, true, opp_bishops_and_queens_mask, opp_pieces, own_pieces, actual_king_sq);
    handle_ray_check_detection_result(nw_ray_result, legality_info, NW, king_sq, actual_king_sq);

    // Check if any opponent knights are attacking the king
    bitmask reverse_knight_attack_mask = attack_tables::knight[king_sq];

    bitmask opp_knights_mask   = is_w ? pos_.bbs.get_b_knights_bb() 
                                      : pos_.bbs.get_w_knights_bb();
    
    // There cannot be more than one knight checking the king
    bitmask knight_attacking_king_mask =  reverse_knight_attack_mask & opp_knights_mask;
    
    // If non-zero, we are in check from the knight
    if ((knight_attacking_king_mask) != 0) {
        sq_idx knight_sq = utils::lsb_idx(knight_attacking_king_mask);
        
        if (actual_king_sq) {
            // Set that we have a checker in the knight square and update check response mask
            utils::set_bit(legality_info.checkers_mask, knight_sq);
            legality_info.check_response_mask = knight_attacking_king_mask;

            // The knight may also be attacking a possible move for the king
            bitmask additional_knight_attack_mask = attack_tables::knight[knight_sq] & attack_tables::king[king_sq];

            if (additional_knight_attack_mask != 0ULL) {
                sq_idx blocking_attack_sq = utils::lsb_idx(additional_knight_attack_mask);
                int dir = offset_to_dir(blocking_attack_sq - king_sq);
                utils::set_bit(legality_info.king_blocked_moves_mask, blocking_attack_sq); 
                legality_info.king_move_offset_response_checklist[dir].first = true;
            }
        } else {
            // The king is in a temporary square and in check, blocked move
            utils::set_bit(legality_info.king_blocked_moves_mask, king_sq);
        }
    }
    
    // Check if any opponent pawns are attacking the king
    bitmask opp_pawns_mask = is_w ? pos_.bbs.get_b_pawns_bb() 
                                  : pos_.bbs.get_w_pawns_bb();
    
    bitmask pawn_capture_mask = is_w ? attack_tables::w_pawn_capture[king_sq] 
                                     : attack_tables::b_pawn_capture[king_sq];

    // There cannot be more than one pawn checking the king
    bitmask pawn_attacking_king_mask = pawn_capture_mask & opp_pawns_mask;

    if (pawn_attacking_king_mask != 0) {
        sq_idx pawn_sq = utils::lsb_idx(pawn_attacking_king_mask);
        
        if (actual_king_sq) {
            utils::set_bit(legality_info.checkers_mask, pawn_sq);
            legality_info.check_response_mask = pawn_attacking_king_mask;
        } else {
            // The king is in a temporary square and in check, blocked move
            utils::set_bit(legality_info.king_blocked_moves_mask, king_sq);
        }
    }

    // Check if the opposing king is blocking movement for ours. Only needed on original square
    if (actual_king_sq) {
        sq_idx opp_king_sq = utils::lsb_idx(is_w ? pos_.bbs.get_b_king_bb()
                                                 : pos_.bbs.get_w_king_bb());
    
        bitmask blocked_squares = attack_tables::king[king_sq] & attack_tables::king[opp_king_sq];
        if (blocked_squares != 0ULL) {
            std::vector<sq_idx> blocked_sqs_idxs;
            utils::get_bit_idxs(blocked_sqs_idxs, blocked_squares);
    
            for (sq_idx blocked_sq_idx : blocked_sqs_idxs) {
                utils::set_bit(legality_info.king_blocked_moves_mask, blocked_sq_idx);
            }
        }
    }
}

bool CheckDetection::in_check_from_line_rays() const
{
    // This function is always called after make_move,
    // and we need to check if the maker is left in check
    bool is_w = !pos_.is_w;

    sq_idx king_sq = utils::lsb_idx(is_w ? pos_.bbs.get_w_king_bb()
                                              : pos_.bbs.get_b_king_bb());

    bitmask opp_pieces = is_w ? pos_.occ_masks.get_b_pieces_mask()
                                   : pos_.occ_masks.get_w_pieces_mask();
    bitmask own_pieces = is_w ? pos_.occ_masks.get_w_pieces_mask()
                                   : pos_.occ_masks.get_b_pieces_mask();

    bitmask opp_rooks_and_queens_mask = is_w ? pos_.bbs.get_b_rooks_bb() | pos_.bbs.get_b_queens_bb()
                                                  : pos_.bbs.get_w_rooks_bb() | pos_.bbs.get_w_queens_bb();

    rays::RayCheckDetectionResult n_ray_result = rays::detect_check_in_ray(king_sq, Direction::N, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces, false);
    if (n_ray_result.checker_sq_idx.has_value()) {
        return true;
    }

    rays::RayCheckDetectionResult e_ray_result = rays::detect_check_in_ray(king_sq, Direction::E, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces, false);
    if (e_ray_result.checker_sq_idx.has_value()) {
        return true;
    }

    rays::RayCheckDetectionResult s_ray_result = rays::detect_check_in_ray(king_sq, Direction::S, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces, false);
    if (s_ray_result.checker_sq_idx.has_value()) {
        return true;
    }

    rays::RayCheckDetectionResult w_ray_result = rays::detect_check_in_ray(king_sq, Direction::W, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces, false);
    if (w_ray_result.checker_sq_idx.has_value()) {
        return true;
    }

    return false;
}

bool CheckDetection::in_check() const {
    LegalityInfo legality_info{};

    // This function is always called after make_move,
    // and we need to check if the maker is left in check
    bool is_w = !pos_.is_w;

    sq_idx king_sq = utils::lsb_idx(is_w ? pos_.bbs.get_w_king_bb()
                                         : pos_.bbs.get_b_king_bb());

    reverse_raycast(king_sq, legality_info, true, is_w);

    // Super special case, if you castle which brings you within 1 square of the opp king
    // the above does catch that since I don't do the reverse raycasts from elsewhere
    bitboard opp_king_bb = is_w ? pos_.bbs.get_b_king_bb()
                                : pos_.bbs.get_w_king_bb();

    if ((attack_tables::king[king_sq] & opp_king_bb) != 0ULL) {
        return true;
    }

    return legality_info.in_check();
}

} // namespace logic