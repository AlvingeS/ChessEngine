#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"

namespace evaluation {
    class Evaluator {
        public:
            Evaluator(board::Bitboards& bitboards);
            float evaluate();
        private:
            board::Bitboards& _bitboards;
    };
}