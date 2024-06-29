#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/RayBitMasks.h"
#include "ChessEngine/masks/KnightBitMasks.h"
#include "ChessEngine/masks/PawnBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class CheckDetection {
        public:
            // Constructor
            CheckDetection(game::ChessBoard& board, RayLogic* rayLogic);
            
            // Public member functions
            bool isInCheck(bool isWhite);
        private:

            // Private member functions
            game::ChessBoard& _board;
            RayLogic* _rayLogic;

            // Memory
            std::vector<masks::StraightRays> _straightRayBitmasks;
            std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
            std::vector<U64> _knightBitmasks;
            std::vector<U64> _whitePawnCaptureMoveBitmasks;
            std::vector<U64> _blackPawnCaptureMoveBitmasks;

    };
}