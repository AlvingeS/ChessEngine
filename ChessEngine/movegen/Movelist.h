#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"

namespace movegen {

class Movelist {

public:
    Movelist() {
        movelist = std::vector<move::Move>(MAX_MOVES);
        moveIndex = 0;
    }

    inline void addMove(const move::Move& move) {
        movelist[moveIndex] = move;
        moveIndex++;
    }

    inline void addNullMove() {
        movelist[moveIndex] = move::Move();
    }

    inline void reset() {
        moveIndex = 0;
    }

    inline const move::Move& getMoveAt(int index) const {
        return movelist[index];
    }

    inline int getMoveIndex() const {
        return moveIndex;
    }

private:
    static constexpr int MAX_MOVES = 218;

    std::vector<move::Move> movelist;
    int moveIndex;
};

} // namespace movegen
