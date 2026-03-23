#include "model/move/movelist.h"

#include "model/constants.h"

namespace model {

Movelist::Movelist() : move_idx_(0) {}

void Movelist::add_move(const Move& move) {
    movelist_[move_idx_] = move;
    move_idx_++;
}

} // namespace model