#include "logic/movegen/bishop_gen.h"

#include "logic/movegen/rays.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"

#include "model/position/position.h"
#include "model/move/move.h"
#include "model/move/movelist.h"

#include "io/board_printer.h"

#include <iostream>

namespace logic {

BishopGen::BishopGen(const model::Position& pos) 
    : pos_(pos)
{}

void BishopGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    if (legality_info.in_double_check()) {
        return;
    }

    bitmask occupied_pieces_mask = pos_.bbs.get_occ();
    bitmask opponent_pieces_mask = pos_.is_w ? pos_.bbs.get_b()
                                             : pos_.bbs.get_w();

    bitboard bb = pos_.is_w ? pos_.bbs.get(PieceType::W_BISHOP)
                            : pos_.bbs.get(PieceType::B_BISHOP);

    utils::for_each_bit(bb, [&](sq_t sq) {
        rays::add_moves_from_ray(Direction::NE, true,  sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info);
        rays::add_moves_from_ray(Direction::SE, false, sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info);
        rays::add_moves_from_ray(Direction::SW, false, sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info);
        rays::add_moves_from_ray(Direction::NW, true,  sq, movelist, occupied_pieces_mask, opponent_pieces_mask, legality_info);
    });
}

} // namespace logic