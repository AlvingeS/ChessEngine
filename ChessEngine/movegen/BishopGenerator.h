#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/RayBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class BishopGenerator {
        public:
            // Constructor
            BishopGenerator(game::ChessBoard& board, RayLogic* rayLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            RayLogic* _rayLogic;

            // Memory
            std::vector<int> _bishopIndices;
            std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
    };
}