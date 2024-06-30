#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"

namespace movegen {
    class CommonLogic {
        public:
            void addMove(int bitIndexFrom, int bitIndexTo, int flag, std::vector<move::Move>& moveList, int& moveIndex) {
                moveList[moveIndex] = move::Move(bitIndexFrom, bitIndexTo, flag);
                moveIndex++;
            }
        private:
    };
}