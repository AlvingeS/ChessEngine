#pragma once
#include <vector>
#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class QueenGenerator {
        public:
            // Constructor
            QueenGenerator(game::ChessBoard& board, RayLogic* rayLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            RayLogic* _rayLogic;

            // Memory
            std::vector<int> _queenIndices;
            std::vector<bits::StraightRays> _straightRayBitmasks;
            std::vector<bits::DiagonalRays> _diagonalRayBitmasks;
    };
}