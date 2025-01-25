#pragma once

namespace model {
    class Board;
    class Bitboards;
}

namespace logic {

class Evaluator {

public:
    Evaluator(model::Board& board);
    float evaluate();

private:
    model::Bitboards& _bitboards;
};

} // namespace logic