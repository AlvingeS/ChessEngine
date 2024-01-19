#pragma once
#include "Move.h"
#include <vector>
#include "ChessBoard.h"

namespace game {
    class MoveGenerator {
        public:
            MoveGenerator();
            std::vector<Move>& genMoves(bool is_white);
            void genRookMoves(bool is_white);

            static constexpr int MAX_LEGAL_MOVES = 218;
        private:
            std::vector<Move> _moves; 
            ChessBoard _board;
    };
}