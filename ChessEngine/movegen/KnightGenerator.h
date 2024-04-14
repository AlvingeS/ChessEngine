#pragma once
#include <vector>
#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/KnightBitMasks.h"
#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class KnightGenerator {
        public:
            // Constructor
            KnightGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            int& _moveIndex;
            CommonLogic* _commonLogic;

            // Memory
            std::vector<int> _knightIndices;
            std::vector<bits::U64> _knightBitmasks;
            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;
    };
}