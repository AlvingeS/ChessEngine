#pragma once
#include <vector>
#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/KingBitMasks.h"
#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class KingGenerator {
        public:
            // Constructor
            KingGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            int& _moveIndex;
            CommonLogic* _commonLogic;

            // Memory
            std::vector<int> _kingIndices;
            std::vector<bits::U64> _kingBitmasks;
            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;
    };
}