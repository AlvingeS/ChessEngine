#include "logic/movegen/knight_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bits.h"

namespace logic {

KnightGen::KnightGen(model::Board& board) 
    : bbs_(board.bbs)
    , occupancy_masks_(board.occupancy_masks)
    , knight_attack_table_(attack_tables::knight)
{}

void KnightGen::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<sq_idx>& knight_sqs        = Containers::get_piece_position_idxs();
    std::vector<sq_idx>& quiet_moves_sqs   = Containers::get_leaping_piece_quiet_moves_idxs();
    std::vector<sq_idx>& capture_moves_sqs = Containers::get_leaping_piece_capturable_moves_idxs();

    bits::get_bit_idxs(knight_sqs, is_w ? bbs_.get_w_knights_bb()
                                             : bbs_.get_b_knights_bb());

    for (int knight_sq : knight_sqs) {
        bitmask attack_mask = knight_attack_table_[knight_sq];

        bitmask quiet_moves_mask = attack_mask & occupancy_masks_.get_free_sqrs_mask();
        
        bitmask opp_pieces_mask = is_w ? occupancy_masks_.get_b_pieces_mask()
                                       : occupancy_masks_.get_w_pieces_mask();
        
        bitmask capture_moves_mask = attack_mask & opp_pieces_mask;

        bits::get_bit_idxs(quiet_moves_sqs, quiet_moves_mask);
        bits::get_bit_idxs(capture_moves_sqs, capture_moves_mask);

        for (sq_idx to_sq : quiet_moves_sqs) {
            movelist.add_move(model::Move(knight_sq, to_sq, model::Move::QUITE_FLAG));
        }

        for (sq_idx to_sq : capture_moves_sqs) {
            movelist.add_move(model::Move(knight_sq, to_sq, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic