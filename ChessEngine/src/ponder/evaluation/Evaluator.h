#pragma once

#include "ChessEngine/src/game/board/Bitboards.h"

namespace ponder {
namespace evaluation {

class Evaluator {

public:
    Evaluator(game::board::Bitboards& bitboards);
    float evaluate();
private:
    game::board::Bitboards& _bitboards;
};

} // namespace evaluation
} // namespace ponder