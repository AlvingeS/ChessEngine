#include "logic/movegen/knight_gen.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"

namespace logic {

KnightGen::KnightGen(const model::Position& pos) 
    : pos_(pos) 
    , knight_attack_table_(attack_tables::knight)
{}

void KnightGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info) 
{
    if (legality_info.in_double_check()) {
        return;
    }

    bitboard knights_bb = pos_.is_w ? pos_.bbs.get_w_knights_bb()
                                    : pos_.bbs.get_b_knights_bb();

    utils::controlled_for_each_bit(knights_bb, [&](sq_t knight_sq)  {
        // A pinned knight cannot move
        if (utils::get_bit(legality_info.pinned_mask, knight_sq)) {
            return LoopControl::Continue;
        }
    
        bitmask legal_moves = knight_attack_table_[knight_sq];
        if (legality_info.in_check()) {
            legal_moves &= legality_info.check_response_mask;
        }
    
        bitmask quiet_moves_mask = legal_moves & pos_.occ_masks.get_free_squares_mask();
        
        bitmask opp_pieces_mask = pos_.is_w ? pos_.occ_masks.get_b_pieces_mask()
                                            : pos_.occ_masks.get_w_pieces_mask();
        
        bitmask capture_moves_mask = legal_moves & opp_pieces_mask;
    
        utils::for_each_bit(quiet_moves_mask, [&](sq_t to_sq) {
            movelist.add_move(model::Move(knight_sq, to_sq, model::Move::QUIET_FLAG));
        });

        utils::for_each_bit(capture_moves_mask, [&](sq_t to_sq) {
            movelist.add_move(model::Move(knight_sq, to_sq, model::Move::CAPTURE_FLAG));
        });

        return LoopControl::Continue;
    });
}

} // namespace logic