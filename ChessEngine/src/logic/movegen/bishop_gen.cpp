#include "logic/movegen/bishop_gen.h"

#include "logic/movegen/ray_logic.h"

#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/utils/chess_utils.h"

#include "model/position/piece_type.h"
#include "model/position/board.h"
#include "model/move/move.h"
#include "model/move/movelist.h"

namespace logic {

BishopGen::BishopGen(model::Board& board) 
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , diag_ray_attack_table_(attack_tables::diag_ray)
{}

void BishopGen::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& bishop_sq_idxs = Containers::get_piece_position_indices();

    BitBasics::get_bit_indices(bishop_sq_idxs, is_w ? bitboards_.get_w_bishops_bb()
                                         : bitboards_.get_b_bishops_bb());

    for (int bishop_sq_idx : bishop_sq_idxs) {
        int bishopRank = ChessUtils::rank_from_bit_index(bishop_sq_idx);
        int bishopFile = ChessUtils::file_from_bit_index(bishop_sq_idx);

        RayLogic::add_moves_from_diag_ray(diag_ray_attack_table_[bishop_sq_idx][DiagDir::NE], true, is_w, bishop_sq_idx, bishopRank, bishopFile, movelist, occupancy_masks_.get_w_pieces_mask(), occupancy_masks_.get_occupied_squares_mask());
        RayLogic::add_moves_from_diag_ray(diag_ray_attack_table_[bishop_sq_idx][DiagDir::SE], false, is_w, bishop_sq_idx, bishopRank, bishopFile, movelist, occupancy_masks_.get_w_pieces_mask(), occupancy_masks_.get_occupied_squares_mask());
        RayLogic::add_moves_from_diag_ray(diag_ray_attack_table_[bishop_sq_idx][DiagDir::SW], false, is_w, bishop_sq_idx, bishopRank, bishopFile, movelist, occupancy_masks_.get_w_pieces_mask(), occupancy_masks_.get_occupied_squares_mask());
        RayLogic::add_moves_from_diag_ray(diag_ray_attack_table_[bishop_sq_idx][DiagDir::NW], true, is_w, bishop_sq_idx, bishopRank, bishopFile, movelist, occupancy_masks_.get_w_pieces_mask(), occupancy_masks_.get_occupied_squares_mask());
    }
}

} // namespace logic