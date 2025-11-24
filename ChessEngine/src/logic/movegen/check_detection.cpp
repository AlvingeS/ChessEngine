#include "logic/movegen/check_detection.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/attack_tables/attack_tables.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : bbs_(board.bbs)
    , occupancy_masks_(board.occupancy_masks)
    , line_ray_attack_table_(attack_tables::line_ray)
    , diag_ray_attack_table_(attack_tables::diag_ray)
    , knight_attack_table_(attack_tables::knight)
    , w_pawn_capture_attack_table_(attack_tables::w_pawn_capture)
    , b_pawn_capture_attack_table_(attack_tables::b_pawn_capture)
{}

bool CheckDetection::in_check(bool is_w) const
{
    sq_idx king_sq, opp_king_sq;
    int king_rank_diff, king_file_diff;

    king_sq = BitBasics::lsb_idx(is_w ? bbs_.get_w_king_bb()
                                      : bbs_.get_b_king_bb());

    // Check if any opponent rooks or queens are attacking the king
    bitmask opp_rooks_and_queens_mask = is_w ? bbs_.get_b_rooks_bb() | bbs_.get_b_queens_bb()
                                             : bbs_.get_w_rooks_bb() | bbs_.get_w_queens_bb();

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq][LineDir::N], true, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq][LineDir::E], false, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq][LineDir::S], false, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq][LineDir::W], true, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    bitmask opp_bishops_and_queens_mask = is_w ? bbs_.get_b_bishops_bb() | bbs_.get_b_queens_bb() 
                                               : bbs_.get_w_bishops_bb() | bbs_.get_w_queens_bb();

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq][DiagDir::NE], true, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq][DiagDir::SE], false, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq][DiagDir::SW], false, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq][DiagDir::NW], true, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_sqrs_mask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knight_attack_mask = knight_attack_table_[king_sq];
    bitmask opp_knights_mask   = is_w ? bbs_.get_b_knights_bb() 
                                      : bbs_.get_w_knights_bb();

    if ((knight_attack_mask & opp_knights_mask) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opp_pawns_mask = is_w ? bbs_.get_b_pawns_bb() 
                                  : bbs_.get_w_pawns_bb();
    
    bitmask pawn_capture_mask = is_w ? w_pawn_capture_attack_table_[king_sq] 
                                     : b_pawn_capture_attack_table_[king_sq];

    if ((pawn_capture_mask & opp_pawns_mask) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opp_king_sq = BitBasics::lsb_idx(is_w ? bbs_.get_b_king_bb()
                                          : bbs_.get_w_king_bb());

    king_rank_diff = ChessUtils::abs(ChessUtils::rank_from_sq(king_sq) - ChessUtils::rank_from_sq(opp_king_sq));
    king_file_diff = ChessUtils::abs(ChessUtils::file_from_sq(king_sq) - ChessUtils::file_from_sq(opp_king_sq));

    int manhattan_distance = king_rank_diff + king_file_diff;

    if (manhattan_distance <= 1)
        return true;

    if (manhattan_distance == 2 && king_rank_diff == 1 && king_file_diff == 1)
        return true;

    return false;
}

} // namespace logic