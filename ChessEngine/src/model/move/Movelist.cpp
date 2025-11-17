#include "model/move/Movelist.h"

#include "model/Constants.h"

namespace model {

Movelist::Movelist() {
    _movelist = std::vector<Move>(MAX_LEGAL_MOVES);
    _moveIndex = 0;
}

void Movelist::addMove(const Move& move) {
    _movelist[_moveIndex] = move;
    _moveIndex++;
}

} // namespace model