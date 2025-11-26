#include "logic/movegen/queen_gen.h"

#include "model/position/board.h"

#include "logic/movegen/rays.h"
#include "logic/movegen/containers.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic {

QueenGen::QueenGen(model::Board& board) 
    : bbs_(board.bbs)
    , occupancy_masks_(board.occupancy_masks)
    , line_ray_attack_table_(attack_tables::line_ray)
    , diag_ray_attack_table_(attack_tables::diag_ray)
{}

void QueenGen::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& queen_sqs = Containers::get_piece_position_idxs();

    utils::get_bit_idxs(queen_sqs, is_w ? bbs_.get_w_queens_bb()
                                            : bbs_.get_b_queens_bb());

    for (int queen_sq : queen_sqs) {
        int rank = utils::rank_from_sq(queen_sq);
        int file = utils::file_from_sq(queen_sq);

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::N],
            true,
            false,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::E],
            false,
            true,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::S],
            false,
            false,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_line_ray(
            line_ray_attack_table_[queen_sq][LineDir::W],
            true,
            true,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::NE],
            true,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::SE],
            false,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );

        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::SW],
            false,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );
        
        rays::add_moves_from_diag_ray(
            diag_ray_attack_table_[queen_sq][DiagDir::NW],
            true,
            is_w,
            queen_sq,
            rank,
            file,
            movelist,
            occupancy_masks_.get_w_pieces_mask(),
            occupancy_masks_.get_occupied_sqrs_mask()
        );
    }
}

} // namespace logic