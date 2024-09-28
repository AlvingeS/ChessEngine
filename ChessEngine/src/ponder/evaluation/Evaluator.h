#pragma once

#include "ChessEngine/src/game/board/Bitboards.h"

namespace evaluation {
    class Evaluator {
        public:
            Evaluator(board::Bitboards& bitboards);
            float evaluate();
        private:
            board::Bitboards& _bitboards;
    };
}