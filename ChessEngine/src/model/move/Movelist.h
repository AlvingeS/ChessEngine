#pragma once

#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/move/Move.h"

namespace model {

class Movelist {

public:
    Movelist() {
        movelist = std::vector<Move>(MAX_MOVES);
        moveIndex = 0;
    }

    inline void addMove(const Move& move) {
        movelist[moveIndex] = move;
        moveIndex++;
    }

    inline void addNullMove() {
        movelist[moveIndex] = Move();
    }

    inline void reset() {
        moveIndex = 0;
    }

    inline const Move& getMoveAt(int index) const {
        return movelist[index];
    }

    inline int getMoveIndex() const {
        return moveIndex;
    }

private:
    static constexpr int MAX_MOVES = 218;

    std::vector<Move> movelist;
    int moveIndex;
};

} // namespace model