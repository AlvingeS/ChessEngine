#pragma once
#include <vector>
#include "ChessEngine/game/Move.h"
#include "ChessEngine/bits/PawnBitMasks.h"
#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class PawnGenerator {
        public:
            // Constructor
            PawnGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::ChessBoard& _board;
            int& _moveIndex;
            CommonLogic* _commonLogic;

            // Memory
            std::vector<int> _pawnIndices;
            std::vector<bits::U64> _whitePawnStraightMoveBitmasks;
            std::vector<bits::U64> _blackPawnStraightMoveBitmasks;
            std::vector<bits::U64> _whitePawnCaptureMoveBitmasks;
            std::vector<bits::U64> _blackPawnCaptureMoveBitmasks;

            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;
    };
}