#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/BitBoards.h"

namespace evaluation {
    class Evaluator {
        public:
            Evaluator(game::BitBoards& bitboards);
            float evaluate();
        private:
            game::BitBoards& _bitboards;
    };
}