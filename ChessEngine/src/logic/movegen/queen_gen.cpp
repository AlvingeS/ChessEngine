#include "logic/movegen/queen_gen.h"

#include "model/position/position.h"

#include "logic/movegen/rays.h"
#include "logic/movegen/containers.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic {

QueenGen::QueenGen(const model::Position& pos) 
    : pos_(pos)   
    , line_ray_attack_table_(attack_tables::line_ray)
    , diag_ray_attack_table_(attack_tables::diag_ray)
{}

void QueenGen::generate(
    model::Movelist& movelist)
{
    std::vector<int>& queen_sqs = Containers::get_piece_position_idxs();

    utils::get_bit_idxs(queen_sqs, pos_.is_w ? pos_.bbs.get_w_queens_bb()
                                             : pos_.bbs.get_b_queens_bb());

    for (int queen_sq : queen_sqs) {
        int rank = utils::rank_from_sq(queen_sq);
        int file = utils::file_from_sq(queen_sq);

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::N],
            true,
            false,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::E],
            true,
            true,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::S],
            false,
            false,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::W],
            false,
            true,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::NE],
            true,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::SE],
            false,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::SW],
            false,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );
        
        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::NW],
            true,
            pos_.is_w,
            queen_sq,
            rank,
            file,
            movelist,
            pos_.occ_masks.get_w_pieces_mask(),
            pos_.occ_masks.get_occupied_squares_mask()
        );
    }
}

} // namespace logic