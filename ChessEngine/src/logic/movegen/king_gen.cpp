#include "logic/movegen/king_gen.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"

namespace logic {

KingGen::KingGen(const model::Position& pos)
    : pos_(pos)
    , king_attack_table_(attack_tables::king)
{}

void KingGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    sq_t king_sq = utils::lsb_idx(pos_.is_w ? pos_.bbs.get(PieceType::W_KING)
                                              : pos_.bbs.get(PieceType::B_KING));

    bitmask legal_moves      = king_attack_table_[king_sq] & legality_info.king_response_mask;
    
    bitmask opp_pieces_mask  = pos_.is_w ? pos_.bbs.get_b() 
                                         : pos_.bbs.get_w();
    
    if (!captures_only) {
        bitmask quiet_moves_mask = legal_moves & pos_.bbs.get_empty();
        
        utils::for_each_bit(quiet_moves_mask, [&](sq_t to_sq)  {
            movelist.add_move(model::Move(king_sq, to_sq, model::Move::QUIET_FLAG));
        });
    }

    bitmask capture_moves_mask = legal_moves & opp_pieces_mask;
    
    utils::for_each_bit(capture_moves_mask, [&](sq_t to_sq) {
        movelist.add_move(model::Move(king_sq, to_sq, model::Move::CAPTURE_FLAG));
    });
}

} // namespace logic