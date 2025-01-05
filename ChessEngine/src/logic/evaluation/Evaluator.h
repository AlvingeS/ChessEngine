#pragma once

#include "ChessEngine/src/model/board/Board.h"

namespace logic {

class Evaluator {

public:
    Evaluator(model::Board& board);
    float evaluate();

private:
    model::Bitboards& _bitboards;
};

} // namespace logic