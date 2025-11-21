#include "logic/movegen/knight_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KnightGen::KnightGen(model::Board& board) 
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , knight_attack_table_(attack_tables::knight)
{}

void KnightGen::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<int>& knight_sq_idxs = Containers::get_piece_position_idxs();
    std::vector<int>& quiet_moves_sq_idxs = Containers::get_leaping_piece_quiet_moves_idxs();
    std::vector<int>& capture_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_idxs();

    BitBasics::get_bit_idxs(knight_sq_idxs, is_w ? bitboards_.get_w_knights_bb()
                                                    : bitboards_.get_b_knights_bb());

    for (int knight_sq_idx : knight_sq_idxs) {
        bitmask attack_mask = knight_attack_table_[knight_sq_idx];

        bitmask quiet_moves_mask = attack_mask & occupancy_masks_.get_free_squares_mask();
        
        bitmask opp_pieces_mask = is_w ? occupancy_masks_.get_b_pieces_mask()
                                       : occupancy_masks_.get_w_pieces_mask();
        
        bitmask capture_moves_mask = attack_mask & opp_pieces_mask;

        BitBasics::get_bit_idxs(quiet_moves_sq_idxs, quiet_moves_mask);
        BitBasics::get_bit_idxs(capture_moves_sq_idxs, capture_moves_mask);

        for (int quiet_move_sq_idx : quiet_moves_sq_idxs) {
            movelist.add_move(model::Move(knight_sq_idx, quiet_move_sq_idx, model::Move::QUITE_FLAG));
        }

        for (int capture_move_sq_idx : capture_moves_sq_idxs) {
            movelist.add_move(model::Move(knight_sq_idx, capture_move_sq_idx, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic