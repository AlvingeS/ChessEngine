#include "logic/movegen/rook_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

RookGenerator::RookGenerator(model::Board& board)
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , line_ray_attack_table_(attack_tables::line_ray)
{}

void RookGenerator::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& rook_sq_idxs = Containers::get_piece_position_indices();

    BitBasics::get_bit_indices(rook_sq_idxs, is_w ? bitboards_.get_w_rooks_bb()
                                       : bitboards_.get_b_rooks_bb());

    // Loop through all rooks and isolate them
    for (int rook_sq_idx : rook_sq_idxs) {
        int rank = ChessUtils::rank_from_bit_idx(rook_sq_idx);
        int file = ChessUtils::file_from_bit_idx(rook_sq_idx);

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq_idx][LineDir::N],
            true, 
            false, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_squares_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq_idx][LineDir::E],
            false, 
            true, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_squares_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq_idx][LineDir::S],
            false, 
            false, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_squares_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq_idx][LineDir::W],
            true, 
            true, 
            is_w, 
            rook_sq_idx, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_squares_mask()
        );
    }
}

} // namespace logic