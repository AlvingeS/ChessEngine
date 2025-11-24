#include "logic/movegen/rook_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

RookGen::RookGen(model::Board& board)
    : bbs_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , line_ray_attack_table_(attack_tables::line_ray)
{}

void RookGen::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<sq_idx>& rook_sqs = Containers::get_piece_position_idxs();

    BitBasics::get_bit_idxs(rook_sqs, is_w ? bbs_.get_w_rooks_bb()
                                           : bbs_.get_b_rooks_bb());

    // Loop through all rooks and isolate them
    for (int rook_sq : rook_sqs) {
        int rank = ChessUtils::rank_from_sq(rook_sq);
        int file = ChessUtils::file_from_sq(rook_sq);

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::N],
            true, 
            false, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::E],
            false, 
            true, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::S],
            false, 
            false, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        RayLogic::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::W],
            true, 
            true, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );
    }
}

} // namespace logic