#include "model/move/movelist.h"

#include "model/constants.h"

namespace model {

Movelist::Movelist() {
    movelist_ = std::vector<Move>(constants::MAX_LEGAL_MOVES);
    move_idx_ = 0;
}

void Movelist::add_move(const Move& move) {
    movelist_[move_idx_] = move;
    move_idx_++;
}

} // namespace model