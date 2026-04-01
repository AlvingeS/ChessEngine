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
    const model::Position& pos_;
};

} // namespace logic