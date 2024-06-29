#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/masks/KingBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {
    class KingGenerator {
        public:
            // Constructor
            KingGenerator(game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitmasks, int& moveIndex, CommonLogic* commonLogic);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList);
        private:

            // Private member functions
            game::BitBoards& _bitboards;
            game::GameStateBitMasks& _gameStateBitmasks;
            int& _moveIndex;
            CommonLogic* _commonLogic;

            // Memory
            std::vector<int> _kingIndices;
            std::vector<U64> _kingBitmasks;
            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;
    };
}