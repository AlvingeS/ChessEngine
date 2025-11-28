#pragma once

namespace model {
    class Position;
}

namespace logic {

class Eval {

public:
    Eval(const model::Position& pos);
    
    float evaluate();

private:
    const model::Position& pos_;
};

} // namespace logic