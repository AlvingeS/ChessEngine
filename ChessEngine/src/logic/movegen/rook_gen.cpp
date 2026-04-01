#include "logic/movegen/rook_gen.h"

#include "model/position/position.h"

#include "logic/movegen/rays.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic {

RookGen::RookGen(const model::Position& pos)
    : pos_(pos)
{}

void RookGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only)
{
    if (legality_info.in_double_check()) {
        return;
    }

    bitmask occupied_pieces_mask = pos_.bbs.get_occ();
    bitmask opponent_pieces_mask = pos_.is_w ? pos_.bbs.get_b()
                                             : pos_.bbs.get_w();

    bitboard bb = pos_.is_w ? pos_.bbs.get(PieceType::W_ROOK)
                            : pos_.bbs.get(PieceType::B_ROOK);

    utils::for_each_bit(bb, [&](sq_t sq) {
        rays::add_moves_from_ray(Direction::N,  true,  sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info, captures_only);
        rays::add_moves_from_ray(Direction::E,  true,  sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info, captures_only);
        rays::add_moves_from_ray(Direction::S,  false, sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info, captures_only);
        rays::add_moves_from_ray(Direction::W,  false, sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info, captures_only);
    });
}

} // namespace logic