#pragma once

#include "ChessEngine/game/Move.h"
#include <vector>

namespace movegen {
    class CommonLogic {
        public:
            void addMove(int bitIndexFrom, int bitIndexTo, int flag, std::vector<game::Move>& moveList, int& moveIndex) {
                moveList[moveIndex] = game::Move(bitIndexFrom, bitIndexTo, flag);
                moveIndex++;
            }
        private:
    };
}