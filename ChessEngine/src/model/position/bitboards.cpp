#include "model/position/board.h"

namespace model {

Bitboards::Bitboards() 
{
    bbs_.reserve(12);

    // TODO: Move to model/masks
    w_pawns_bb_   = 0x000000000000FF00ULL;
    w_knights_bb_ = 0x0000000000000042ULL;
    w_bishops_bb_ = 0x0000000000000024ULL;
    w_rooks_bb_   = 0x0000000000000081ULL;
    w_queens_bb_  = 0x0000000000000010ULL;
    w_king_bb_    = 0x0000000000000008ULL;

    b_pawns_bb_   = 0x00FF000000000000ULL;
    b_knights_bb_ = 0x4200000000000000ULL;
    b_bishops_bb_ = 0x2400000000000000ULL;
    b_rooks_bb_   = 0x8100000000000000ULL;
    b_queens_bb_  = 0x1000000000000000ULL;
    b_king_bb_    = 0x0800000000000000ULL;

    bbs_.push_back(&w_pawns_bb_);
    bbs_.push_back(&w_knights_bb_);
    bbs_.push_back(&w_bishops_bb_);
    bbs_.push_back(&w_rooks_bb_);
    bbs_.push_back(&w_queens_bb_);
    bbs_.push_back(&w_king_bb_);

    bbs_.push_back(&b_pawns_bb_);
    bbs_.push_back(&b_knights_bb_);
    bbs_.push_back(&b_bishops_bb_);
    bbs_.push_back(&b_rooks_bb_);
    bbs_.push_back(&b_queens_bb_);
    bbs_.push_back(&b_king_bb_);
}

void Bitboards::reset_bitboards() 
{
    for (int i = 0; i < 12; i++)
        *(bbs_[i]) = 0;
}

} // namespace model