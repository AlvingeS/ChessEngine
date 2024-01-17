#pragma once
#include "move.h"
#include <vector>
#include "chessboard.h"

namespace game {
    class MoveGenerator {
        public:
            MoveGenerator();
            std::vector<Move>& generate_moves(bool is_white);
            void generate_rook_moves(bool is_white);

            static constexpr int MAX_LEGAL_MOVES = 218;
        private:
            std::vector<Move> m_moves; 
            ChessBoard m_board;
    };
}