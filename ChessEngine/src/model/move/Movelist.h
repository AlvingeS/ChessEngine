#pragma once

#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/move/Move.h"

namespace model {

class Movelist {

public:
    Movelist();

    void addMove(const Move& move);

    inline void addNullMove() {
        _movelist[_moveIndex] = Move();
    }

    inline void reset() {
        _moveIndex = 0;
    }

    inline const Move& getMoveAt(int index) const {
        return _movelist[index];
    }

    inline int getMoveIndex() const {
        return _moveIndex;
    }

private:
    std::vector<Move> _movelist;
    int _moveIndex;
};

} // namespace model