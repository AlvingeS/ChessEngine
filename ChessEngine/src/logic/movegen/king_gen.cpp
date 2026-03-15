#include "logic/movegen/king_gen.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/check_detection.h"
#include "logic/movegen/containers.h"
#include "logic/utils.h"

namespace logic {

KingGen::KingGen(const model::Position& pos)
    : pos_(pos)
    , king_attack_table_(attack_tables::king)
{}

void KingGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info) 
{
    std::vector<sq_idx>& king_sqs             = Containers::get_piece_position_idxs();
    std::vector<sq_idx>& quiet_moves_sqs      = Containers::get_leaping_piece_quiet_moves_idxs();
    std::vector<sq_idx>& capturable_moves_sqs = Containers::get_leaping_piece_capturable_moves_idxs();
    
    utils::get_bit_idxs(king_sqs, pos_.is_w ? pos_.bbs.get_w_king_bb()
                                            : pos_.bbs.get_b_king_bb());

    int king_sq_idx = king_sqs[0];

    bitmask legal_moves      = king_attack_table_[king_sq_idx] & legality_info.king_response_mask;
    bitmask quiet_moves_mask = legal_moves & pos_.occ_masks.get_free_squares_mask();
    
    bitmask opp_pieces_mask = pos_.is_w ? pos_.occ_masks.get_b_pieces_mask() 
                                        : pos_.occ_masks.get_w_pieces_mask();

    bitmask capture_moves_mask = legal_moves & opp_pieces_mask;

    utils::get_bit_idxs(quiet_moves_sqs, quiet_moves_mask);
    utils::get_bit_idxs(capturable_moves_sqs, capture_moves_mask);

    for (sq_idx to_sq : quiet_moves_sqs) {
        movelist.add_move(model::Move(king_sq_idx, to_sq, model::Move::QUITE_FLAG));
    }

    for (sq_idx to_sq : capturable_moves_sqs) {
        movelist.add_move(model::Move(king_sq_idx, to_sq, model::Move::CAPTURE_FLAG));
    }
}

} // namespace logic