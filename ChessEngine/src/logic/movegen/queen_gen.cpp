#include "logic/movegen/queen_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/utils/chess_utils.h"

namespace logic {

QueenGenerator::QueenGenerator(model::Board& board) 
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks)
{
    line_ray_masks_ = RayBitmasks::get_all_straight_ray_bitmasks();
    diag_ray_masks = RayBitmasks::get_all_diagonal_ray_bitmasks();
}

void QueenGenerator::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& queen_sq_idxs = Containers::get_piece_position_indices();

    BitBasics::get_bit_indices(queen_sq_idxs, is_w ? bitboards_.get_w_queens_bitboard()
                                               : bitboards_.get_b_queens_bitboard());

    for (int queen_sq_idx : queen_sq_idxs) {
        RayBitmasks::StraightRays straightRays = line_ray_masks_[queen_sq_idx];
        RayBitmasks::DiagonalRays diagonalRays = diag_ray_masks[queen_sq_idx];
        
        int rank = ChessUtils::rank_from_bit_index(queen_sq_idx);
        int file = ChessUtils::file_from_bit_index(queen_sq_idx);

        RayLogic::add_moves_from_line_ray(
            straightRays.n,
            true,
            false,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            straightRays.e,
            false,
            true,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            straightRays.s,
            false,
            false,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            straightRays.w,
            true,
            true,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_diag_ray(
            diagonalRays.ne,
            true,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_diag_ray(
            diagonalRays.se,
            false,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_diag_ray(
            diagonalRays.sw,
            false,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );
        
        RayLogic::add_moves_from_diag_ray(
            diagonalRays.nw,
            true,
            is_w,
            queen_sq_idx,
            rank,
            file,
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );
    }
}

} // namespace logic