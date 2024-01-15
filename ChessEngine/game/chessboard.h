#pragma once
#include <unordered_map>
#include "ChessEngine/game/piecetype.h"
#include "ChessEngine/utils/bit_logic.h"

namespace game {
    class ChessBoard {
        public:
            ChessBoard();

            private:
                std::unordered_map<PieceType, utils::U64> bitboards;

                bool w_to_move;
    };
}