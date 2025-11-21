#include "logic/movegen/king_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KingGen::KingGen(model::Board& board) 
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , king_attack_table_(attack_tables::king)
{
}

void KingGen::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<sq_idx>& king_sqs             = Containers::get_piece_position_idxs();
    std::vector<sq_idx>& quiet_moves_sqs      = Containers::get_leaping_piece_quiet_moves_idxs();
    std::vector<sq_idx>& capturable_moves_sqs = Containers::get_leaping_piece_capturable_moves_idxs();
    
    BitBasics::get_bit_idxs(king_sqs, is_w ? bitboards_.get_w_king_bb()
                                           : bitboards_.get_b_king_bb());

    int king_sq_idx = king_sqs[0];

    bitmask attack_mask      = king_attack_table_[king_sq_idx];
    bitmask quiet_moves_mask = attack_mask & occupancy_masks_.get_free_sqrs_mask();
    
    bitmask opp_pieces_mask = is_w ? occupancy_masks_.get_b_pieces_mask() 
                                   : occupancy_masks_.get_w_pieces_mask();

    bitmask capture_moves_mask = attack_mask & opp_pieces_mask;

    BitBasics::get_bit_idxs(quiet_moves_sqs, quiet_moves_mask);
    BitBasics::get_bit_idxs(capturable_moves_sqs, capture_moves_mask);

    for (sq_idx to_sq : quiet_moves_sqs) {
        movelist.add_move(model::Move(king_sq_idx, to_sq, model::Move::QUITE_FLAG));
    }

    for (sq_idx to_sq : capturable_moves_sqs) {
        movelist.add_move(model::Move(king_sq_idx, to_sq, model::Move::CAPTURE_FLAG));
    }
}

} // namespace logic