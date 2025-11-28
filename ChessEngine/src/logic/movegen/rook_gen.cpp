#include "logic/movegen/rook_gen.h"

#include "model/position/position.h"

#include "logic/movegen/rays.h"
#include "logic/movegen/containers.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic {

RookGen::RookGen(const model::Position& pos)
    : pos_(pos)
    , line_ray_attack_table_(attack_tables::line_ray)
{}

void RookGen::generate(bool is_w, model::Movelist& movelist)
{
    std::vector<sq_idx>& rook_sqs = Containers::get_piece_position_idxs();

    utils::get_bit_idxs(rook_sqs, is_w ? pos_.bbs.get_w_rooks_bb()
                                       : pos_.bbs.get_b_rooks_bb());

    // Loop through all rooks and isolate them
    for (int rook_sq : rook_sqs) {
        int rank = utils::rank_from_sq(rook_sq);
        int file = utils::file_from_sq(rook_sq);

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::N],
            true, 
            false, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::E],
            false, 
            true, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::S],
            false, 
            false, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[rook_sq][LineDir::W],
            true, 
            true, 
            is_w, 
            rook_sq, 
            rank, 
            file, 
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );
    }
}

} // namespace logic