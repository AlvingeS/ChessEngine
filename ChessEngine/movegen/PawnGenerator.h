#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/PawnBitMasks.h"
#include "ChessEngine/perft/SearchMemory.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class PawnGenerator {
        public:
            // Constructor
            PawnGenerator(game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitmasks, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, perft::SearchMemory& searchMemory);
        private:

            // Private member functions
            game::BitBoards& _bitboards;
            game::GameStateBitMasks& _gameStateBitmasks;
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