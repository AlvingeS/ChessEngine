#include "model/move/movelist.h"

#include "model/constants.h"

namespace model {

Movelist::Movelist() {
    movelist_ = std::vector<Move>(MAX_LEGAL_MOVES);
    move_index_ = 0;
}

void Movelist::add_move(const Move& move) {
    movelist_[move_index_] = move;
    move_index_++;
}

} // namespace model