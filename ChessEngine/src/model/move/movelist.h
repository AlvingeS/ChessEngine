#pragma once

#include "model/move/move.h"
#include "model/constants.h"

#include <algorithm>

namespace model {

class Movelist {

public:
    Movelist();

    const Move& get_move_at(int idx) const;
    int get_move_idx() const;
    
    void add_null_move();
    void add_move(const Move& move);
    void reset();
    void swap(int i, int j);
    
private:
    std::array<Move, constants::MAX_LEGAL_MOVES> movelist_;
    int move_idx_;
};

} // namespace model

#include "model/move/movelist.inl"