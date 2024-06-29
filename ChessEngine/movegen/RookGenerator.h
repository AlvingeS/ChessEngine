#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/RayBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class RookGenerator {
        public:
            // Constructor
            RookGenerator(game::BitBoards& bitboards, RayLogic* rayLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::BitBoards& _bitboards;
            RayLogic* _rayLogic;

            // Memory
            std::vector<int> _rookIndices;
            std::vector<masks::StraightRays> _straightRayBitmasks;
    };
}