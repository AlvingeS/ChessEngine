#pragma once

#include "model/types.h"

namespace model {
    class Position;
}

namespace logic {

class Eval {

public:
    Eval(const model::Position& pos);
    
    eval_t evaluate() const;

private:
    int get_phase() const;
    eval_t get_tapered_king_eval(sq_t sq_idx, bool mirror) const;

    const model::Position& pos_;
};

} // namespace logic