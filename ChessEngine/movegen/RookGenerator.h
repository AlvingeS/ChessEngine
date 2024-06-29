#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/RayBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class RookGenerator {
        public:
            // Constructor
            RookGenerator(game::ChessBoard& board, RayLogic* rayLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            RayLogic* _rayLogic;

            // Memory
            std::vector<int> _rookIndices;
            std::vector<bits::StraightRays> _straightRayBitmasks;
    };
}