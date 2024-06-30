#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/BitBoards.h"

namespace evaluation {
    class Evaluator {
        public:
            Evaluator(board::BitBoards& bitboards);
            float evaluate();
        private:
            board::BitBoards& _bitboards;
    };
}