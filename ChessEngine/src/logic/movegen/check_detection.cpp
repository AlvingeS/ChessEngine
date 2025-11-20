#include "logic/movegen/check_detection.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/attack_tables/attack_tables.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , line_ray_attack_table_(attack_tables::line_ray)
    , diag_ray_attack_table_(attack_tables::diag_ray)
    , knight_attack_table_(attack_tables::knight)
    , w_pawn_capture_attack_table_(attack_tables::w_pawn_capture)
    , b_pawn_capture_attack_table_(attack_tables::b_pawn_capture)
{}

bool CheckDetection::in_check(bool is_w) const
{
    int king_sq_idx, opp_king_sq_idx, king_rank_diff, king_file_diff;

    king_sq_idx = BitBasics::lsb_idx(is_w ? bitboards_.get_w_king_bb()
                                   : bitboards_.get_b_king_bb());

    // Check if any opponent rooks or queens are attacking the king
    bitmask opp_rooks_and_queens_mask = is_w ? bitboards_.get_b_rooks_bb() | bitboards_.get_b_queens_bb()
                                             : bitboards_.get_w_rooks_bb() | bitboards_.get_w_queens_bb();

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq_idx][LineDir::N], true, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq_idx][LineDir::E], false, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq_idx][LineDir::S], false, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_line_ray(line_ray_attack_table_[king_sq_idx][LineDir::W], true, opp_rooks_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    bitmask opp_bishops_and_queens_mask = is_w ? bitboards_.get_b_bishops_bb() | bitboards_.get_b_queens_bb() 
                                               : bitboards_.get_w_bishops_bb() | bitboards_.get_w_queens_bb();

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq_idx][DiagDir::NE], true, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq_idx][DiagDir::SE], false, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq_idx][DiagDir::SW], false, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    if (RayLogic::check_diag_ray(diag_ray_attack_table_[king_sq_idx][DiagDir::NW], true, opp_bishops_and_queens_mask, occupancy_masks_.get_occupied_squares_mask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knight_attack_mask = knight_attack_table_[king_sq_idx];
    bitmask opp_knights_mask = is_w ? bitboards_.get_b_knights_bb() 
                                      : bitboards_.get_w_knights_bb();

    if ((knight_attack_mask & opp_knights_mask) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opp_pawns_mask = is_w ? bitboards_.get_b_pawns_bb() 
                                    : bitboards_.get_w_pawns_bb();
    
    bitmask pawn_capture_mask = is_w ? w_pawn_capture_attack_table_[king_sq_idx] 
                                     : b_pawn_capture_attack_table_[king_sq_idx];

    if ((pawn_capture_mask & opp_pawns_mask) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opp_king_sq_idx = BitBasics::lsb_idx(is_w ? bitboards_.get_b_king_bb()
                                           : bitboards_.get_w_king_bb());

    king_rank_diff = ChessUtils::abs(ChessUtils::rank_from_bit_idx(king_sq_idx) - ChessUtils::rank_from_bit_idx(opp_king_sq_idx));
    king_file_diff = ChessUtils::abs(ChessUtils::file_from_bit_idx(king_sq_idx) - ChessUtils::file_from_bit_idx(opp_king_sq_idx));

    int manhattan_distance = king_rank_diff + king_file_diff;

    if (manhattan_distance <= 1)
        return true;

    if (manhattan_distance == 2 && king_rank_diff == 1 && king_file_diff == 1)
        return true;

    return false;
}

} // namespace logic