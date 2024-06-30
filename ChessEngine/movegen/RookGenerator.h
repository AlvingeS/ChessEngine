#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class RookGenerator {
        public:
            // Constructor
            RookGenerator(board::BitBoards& bitboards, RayLogic* rayLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<move::Move>& moveList);
        private:

            // Private member functions
            board::BitBoards& _bitboards;
            RayLogic* _rayLogic;

            // Memory
            std::vector<int> _rookIndices;
            std::vector<masks::StraightRays> _straightRayBitmasks;
    };
}