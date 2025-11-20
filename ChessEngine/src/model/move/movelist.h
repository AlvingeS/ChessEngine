#pragma once

#include "model/move/move.h"

namespace model {

class Movelist {

public:
    Movelist();

    const Move& get_move_at(int idx) const;
    int get_move_idx() const;
    
    void add_move(const Move& move);
    void add_null_move();
    void reset();
    
private:
    std::vector<Move> movelist_;
    int move_idx_;
};

} // namespace model

#include "model/move/movelist.inl"