#include "logic/movegen/check_detection.h"

#include "model/position/position.h"

#include "logic/movegen/rays.h"
#include "logic/utils.h"
#include "logic/utils.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/attack_tables/attack_tables.h"

namespace logic {

struct LegalityInfo {
    bitmask enemy_attacked_squares = 0ULL;
    bitmask checkers_mask = 0ULL;
    std::array<bitmask, 64> check_attack_masks;
    bitmask pinned_mask = 0ULL;
    std::array<bitmask, 64> pin_rays{};

    bool in_check() {return checkers_mask != 0ULL;};
    bool double_check() {return utils::pop_count(checkers_mask) == 2;};
};

CheckDetection::CheckDetection(const model::Position& pos)
    : pos_(pos)   
    , line_ray_attack_table_(attack_tables::line_ray)
    , diag_ray_attack_table_(attack_tables::diag_ray)
    , knight_attack_table_(attack_tables::knight)
    , w_pawn_capture_attack_table_(attack_tables::w_pawn_capture)
    , b_pawn_capture_attack_table_(attack_tables::b_pawn_capture)
{}

bool CheckDetection::in_check(std::optional<bool> is_w_override) const
{
    LegalityInfo legality_info;
    bool is_w = is_w_override.value_or(pos_.is_w);
    sq_idx king_sq, opp_king_sq;
    int king_rank_diff, king_file_diff;

    king_sq = utils::lsb_idx(is_w ? pos_.bbs.get_w_king_bb()
                                  : pos_.bbs.get_b_king_bb());

    bitmask opp_pieces = is_w ? pos_.occ_masks.get_b_pieces_mask()
                              : pos_.occ_masks.get_w_pieces_mask();
    bitmask own_pieces = is_w ? pos_.occ_masks.get_w_pieces_mask()
                              : pos_.occ_masks.get_b_pieces_mask();

    // Check if any opponent rooks or queens are attacking the king
    bitmask opp_rooks_and_queens_mask = is_w ? pos_.bbs.get_b_rooks_bb() | pos_.bbs.get_b_queens_bb()
                                             : pos_.bbs.get_w_rooks_bb() | pos_.bbs.get_w_queens_bb();

    rays::RayCheckDetectionResult n_ray_result = rays::detect_check_in_line_ray(king_sq, LineDir::N, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces);
    if (n_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, n_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[n_ray_result.checker_sq_idx.value()] = n_ray_result.check_mask.value();
    } else if (n_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, n_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[n_ray_result.pinned_piece_sq_idx.value()] = n_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult e_ray_result = rays::detect_check_in_line_ray(king_sq, LineDir::E, true, opp_rooks_and_queens_mask, opp_pieces, own_pieces);
    if (e_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, e_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[e_ray_result.checker_sq_idx.value()] = e_ray_result.check_mask.value();
    } else if (e_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, e_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[e_ray_result.pinned_piece_sq_idx.value()] = e_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult s_ray_result = rays::detect_check_in_line_ray(king_sq, LineDir::S, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces);
    if (s_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, s_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[s_ray_result.checker_sq_idx.value()] = s_ray_result.check_mask.value();
    } else if (s_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, s_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[s_ray_result.pinned_piece_sq_idx.value()] = s_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult w_ray_result = rays::detect_check_in_line_ray(king_sq, LineDir::W, false, opp_rooks_and_queens_mask, opp_pieces, own_pieces);
    if (w_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, w_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[w_ray_result.checker_sq_idx.value()] = w_ray_result.check_mask.value();
    } else if (w_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, w_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[w_ray_result.pinned_piece_sq_idx.value()] = w_ray_result.pin_ray.value();
    }

    // Check if any opponent bishops or queens are attacking the king
    bitmask opp_bishops_and_queens_mask = is_w ? pos_.bbs.get_b_bishops_bb() | pos_.bbs.get_b_queens_bb() 
                                               : pos_.bbs.get_w_bishops_bb() | pos_.bbs.get_w_queens_bb();

    rays::RayCheckDetectionResult ne_ray_result = rays::detect_check_in_diag_ray(king_sq, DiagDir::NE, true, opp_bishops_and_queens_mask, opp_pieces, own_pieces);
    if (ne_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, ne_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[ne_ray_result.checker_sq_idx.value()] = ne_ray_result.check_mask.value();
    } else if (ne_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, ne_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[ne_ray_result.pinned_piece_sq_idx.value()] = ne_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult se_ray_result = rays::detect_check_in_diag_ray(king_sq, DiagDir::SE, false, opp_bishops_and_queens_mask, opp_pieces, own_pieces);
    if (se_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, se_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[se_ray_result.checker_sq_idx.value()] = se_ray_result.check_mask.value();
    } else if (se_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, se_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[se_ray_result.pinned_piece_sq_idx.value()] = se_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult sw_ray_result = rays::detect_check_in_diag_ray(king_sq, DiagDir::SW, false, opp_bishops_and_queens_mask, opp_pieces, own_pieces);
    if (sw_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, sw_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[sw_ray_result.checker_sq_idx.value()] = sw_ray_result.check_mask.value();
    } else if (sw_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, sw_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[sw_ray_result.pinned_piece_sq_idx.value()] = sw_ray_result.pin_ray.value();
    }

    rays::RayCheckDetectionResult nw_ray_result = rays::detect_check_in_diag_ray(king_sq, DiagDir::NW, true, opp_bishops_and_queens_mask, opp_pieces, own_pieces);
    if (nw_ray_result.checker_sq_idx.has_value()) {
        utils::set_bit(legality_info.checkers_mask, nw_ray_result.checker_sq_idx.value());
        legality_info.check_attack_masks[nw_ray_result.checker_sq_idx.value()] = nw_ray_result.check_mask.value();
    } else if (nw_ray_result.pinned_piece_sq_idx.has_value()) {
        utils::set_bit(legality_info.pinned_mask, nw_ray_result.pinned_piece_sq_idx.value());
        legality_info.pin_rays[nw_ray_result.pinned_piece_sq_idx.value()] = nw_ray_result.pin_ray.value();
    }

    // Check if any opponent knights are attacking the king
    bitmask reverse_knight_attack_mask = knight_attack_table_[king_sq];

    bitmask opp_knights_mask   = is_w ? pos_.bbs.get_b_knights_bb() 
                                      : pos_.bbs.get_w_knights_bb();
    
    // There cannot be more than one knight checking the king
    bitmask knight_attacking_king_mask =  reverse_knight_attack_mask & opp_knights_mask;
    
    if ((knight_attacking_king_mask) != 0) {
        sq_idx knight_sq = utils::lsb_idx(knight_attacking_king_mask);
        utils::set_bit(legality_info.checkers_mask, knight_sq);
        legality_info.check_attack_masks[knight_sq] = knight_attacking_king_mask;
    }
    
    // Check if any opponent pawns are attacking the king
    bitmask opp_pawns_mask = is_w ? pos_.bbs.get_b_pawns_bb() 
                                  : pos_.bbs.get_w_pawns_bb();
    
    bitmask pawn_capture_mask = is_w ? w_pawn_capture_attack_table_[king_sq] 
                                     : b_pawn_capture_attack_table_[king_sq];

    // There cannot be more than one pawn checking the king
    bitmask pawn_attacking_king_mask = pawn_capture_mask & opp_pawns_mask;

    if (pawn_attacking_king_mask != 0) {
        sq_idx pawn_sq = utils::lsb_idx(pawn_attacking_king_mask);
        utils::set_bit(legality_info.checkers_mask, pawn_sq);
        legality_info.check_attack_masks[pawn_sq] = pawn_attacking_king_mask;
    }

    // TEMPORARY, ALL CODE BELOW WILL BE REMOVED AFTER STUPID MAKE/UNMAKE CHECK DETECTION IS REMOVED

    // Check if the king is in check from an adjacent king
    opp_king_sq = utils::lsb_idx(is_w ? pos_.bbs.get_b_king_bb()
                                      : pos_.bbs.get_w_king_bb());

    king_rank_diff = utils::abs(utils::rank_from_sq(king_sq) - utils::rank_from_sq(opp_king_sq));
    king_file_diff = utils::abs(utils::file_from_sq(king_sq) - utils::file_from_sq(opp_king_sq));

    int manhattan_distance = king_rank_diff + king_file_diff;

    if (manhattan_distance <= 1) {
        legality_info.checkers_mask = 1ULL; // Temporary just to be able to do in_check()
    }

    if (manhattan_distance == 2 && king_rank_diff == 1 && king_file_diff == 1) {
        legality_info.checkers_mask = 1ULL; // // Temporary just to be able to do in_check()
    }

    return legality_info.in_check();
}

} // namespace logic