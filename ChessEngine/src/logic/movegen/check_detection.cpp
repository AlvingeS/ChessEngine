#include "logic/movegen/check_detection.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/bitmasks/knight_bitmasks.h"
#include "logic/movegen/bitmasks/pawn_bitmasks.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks)
{
    line_ray_masks_ = RayBitmasks::get_all_straight_ray_bitmasks();
    diag_ray_masks = RayBitmasks::get_all_diagonal_ray_bitmasks();
    knight_masks_ = KnightBitmasks::get_all_knight_bitmasks();
    w_pawn_capture_move_masks = PawnBitmasks::get_all_capture_pawn_move_bitmasks(true);
    b_pawn_capture_move_masks = PawnBitmasks::get_all_capture_pawn_move_bitmasks(false);
}

bool CheckDetection::in_check(bool is_w) const
{
    int king_idx, opponent_king_idx, king_rank_diff, king_file_diff;

    king_idx = BitBasics::lsb_index(is_w ? bitboards_.get_w_king_bitboard()
                                   : bitboards_.get_b_king_bitboard());

    // Check if any opponent rooks or queens are attacking the king
    RayBitmasks::StraightRays straightRays = line_ray_masks_[king_idx];
    bitmask opponentRooksAndQueens = is_w ? bitboards_.get_b_rooks_bitboard() | bitboards_.get_b_queens_bitboard()
                                             : bitboards_.get_w_rooks_bitboard() | bitboards_.get_w_queens_bitboard();

    if (RayLogic::check_line_ray(straightRays.n, true, opponentRooksAndQueens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_line_ray(straightRays.e, false, opponentRooksAndQueens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_line_ray(straightRays.s, false, opponentRooksAndQueens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_line_ray(straightRays.w, true, opponentRooksAndQueens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    RayBitmasks::DiagonalRays diag_rays = diag_ray_masks[king_idx];
    bitmask opp_bishops_and_queens = is_w ? bitboards_.get_b_bishops_bitboard() | bitboards_.get_b_queens_bitboard() 
                                               : bitboards_.get_w_bishops_bitboard() | bitboards_.get_w_queens_bitboard();

    if (RayLogic::check_diag_ray(diag_rays.ne, true, opp_bishops_and_queens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_diag_ray(diag_rays.se, false, opp_bishops_and_queens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_diag_ray(diag_rays.sw, false, opp_bishops_and_queens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    if (RayLogic::check_diag_ray(diag_rays.nw, true, opp_bishops_and_queens, state_bitmasks_.get_occupied_pieces_bitmask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knight_moves = knight_masks_[king_idx];
    bitmask opp_knights = is_w ? bitboards_.get_b_knights_bitboard() 
                                      : bitboards_.get_w_knights_bitboard();

    if ((knight_moves & opp_knights) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opp_pawns = is_w ? bitboards_.get_b_pawns_bitboard() 
                                    : bitboards_.get_w_pawns_bitboard();
    
    bitmask pawn_attacking_moves = is_w ? w_pawn_capture_move_masks[king_idx] 
                                         : b_pawn_capture_move_masks[king_idx];

    if ((pawn_attacking_moves & opp_pawns) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opponent_king_idx = BitBasics::lsb_index(is_w ? bitboards_.get_b_king_bitboard()
                                           : bitboards_.get_w_king_bitboard());

    king_rank_diff = ChessUtils::abs(ChessUtils::rank_from_bit_index(king_idx) - ChessUtils::rank_from_bit_index(opponent_king_idx));
    king_file_diff = ChessUtils::abs(ChessUtils::file_from_bit_index(king_idx) - ChessUtils::file_from_bit_index(opponent_king_idx));

    int manhattan_distance = king_rank_diff + king_file_diff;

    if (manhattan_distance <= 1)
        return true;

    if (manhattan_distance == 2 && king_rank_diff == 1 && king_file_diff == 1)
        return true;

    return false;
}

} // namespace logic