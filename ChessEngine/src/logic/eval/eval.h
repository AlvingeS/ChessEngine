#pragma once

namespace model {
    class Board;
    class Bitboards;
}

namespace logic {

class Eval {

public:
    Eval(model::Board& board);
    
    float evaluate();

private:
    model::Bitboards& bitboards_;
};

} // namespace logic