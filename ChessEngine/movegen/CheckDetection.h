#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/KnightBitMasks.h"
#include "ChessEngine/bits/PawnBitMasks.h"

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
            std::vector<bits::StraightRays> _straightRayBitmasks;
            std::vector<bits::DiagonalRays> _diagonalRayBitmasks;
            std::vector<U64> _knightBitmasks;
            std::vector<U64> _whitePawnCaptureMoveBitmasks;
            std::vector<U64> _blackPawnCaptureMoveBitmasks;

    };
}