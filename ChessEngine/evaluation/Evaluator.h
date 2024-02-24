#pragma once
#include "ChessEngine/game/ChessBoard.h"

namespace evaluation {
    class Evaluator {
        public:
            Evaluator(game::ChessBoard& board);
            int evaluate();
        private:
            game::ChessBoard _board;
    };
}