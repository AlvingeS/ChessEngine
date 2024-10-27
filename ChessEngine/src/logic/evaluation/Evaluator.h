#pragma once

#include "ChessEngine/src/representation/board/Bitboards.h"

namespace logic {
namespace evaluation {

class Evaluator {

public:
    Evaluator(representation::board::Bitboards& bitboards);
    float evaluate();
private:
    representation::board::Bitboards& _bitboards;
};

} // namespace evaluation
} // namespace logic