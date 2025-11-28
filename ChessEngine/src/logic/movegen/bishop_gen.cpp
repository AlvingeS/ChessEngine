#include "logic/movegen/bishop_gen.h"

#include "logic/movegen/rays.h"

#include "logic/movegen/containers.h"
#include "logic/utils.h"

#include "model/position/position.h"
#include "model/move/move.h"
#include "model/move/movelist.h"

namespace logic {

BishopGen::BishopGen(const model::Position& pos) 
    : pos_(pos)
    , diag_ray_attack_table_(attack_tables::diag_ray)
{}

void BishopGen::generate(bool is_w, model::Movelist& movelist)
{
    std::vector<sq_idx>& bishop_sqs = Containers::get_piece_position_idxs();

    utils::get_bit_idxs(bishop_sqs, is_w ? pos_.bbs.get_w_bishops_bb()
                                         : pos_.bbs.get_b_bishops_bb());

    for (sq_idx sq : bishop_sqs) {
        int rank = utils::rank_from_sq(sq);
        int file = utils::file_from_sq(sq);

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[sq][DiagDir::NE],
            true,
            is_w,
            sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[sq][DiagDir::SE],
            false,
            is_w,
            sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[sq][DiagDir::SW],
            false,
            is_w,
            sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[sq][DiagDir::NW],
            true,
            is_w,
            sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

    }
}

} // namespace logic