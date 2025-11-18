#include "logic/movegen/rook_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

RookGenerator::RookGenerator(model::Board& board)
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks)
{
    line_ray_masks_ = RayBitmasks::get_all_straight_ray_bitmasks();
}

void RookGenerator::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& rook_sq_idxs = Containers::get_piece_position_indices();

    RayBitmasks::StraightRays rays;

    BitBasics::get_bit_indices(rook_sq_idxs, is_w ? bitboards_.get_w_rooks_bitboard()
                                       : bitboards_.get_b_rooks_bitboard());

    // Loop through all rooks and isolate them
    for (int rook_sq_idx : rook_sq_idxs) {
        rays = line_ray_masks_[rook_sq_idx];
        int rank = ChessUtils::rank_from_bit_index(rook_sq_idx);
        int file = ChessUtils::file_from_bit_index(rook_sq_idx);

        RayLogic::add_moves_from_line_ray(
            rays.n,
            true, 
            false, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            rays.e,
            false, 
            true, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            rays.s,
            false, 
            false, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );

        RayLogic::add_moves_from_line_ray(
            rays.w,
            true, 
            true, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            state_bitmasks_.get_w_pieces_bitmask(),
            state_bitmasks_.get_occupied_pieces_bitmask()
        );
    }
}

} // namespace logic