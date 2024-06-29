#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/PawnBitMasks.h"
#include "ChessEngine/search/SearchMemory.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class PawnGenerator {
        public:
            // Constructor
            PawnGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, search::SearchMemory& searchMemory);
        private:

            // Private member functions
            game::ChessBoard& _board;
            int& _moveIndex;
            CommonLogic* _commonLogic;

            // Memory
            std::vector<int> _pawnIndices;
            std::vector<U64> _whitePawnStraightMoveBitmasks;
            std::vector<U64> _blackPawnStraightMoveBitmasks;
            std::vector<U64> _whitePawnCaptureMoveBitmasks;
            std::vector<U64> _blackPawnCaptureMoveBitmasks;

            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;
    };
}