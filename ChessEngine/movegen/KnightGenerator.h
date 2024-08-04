#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/KnightBitMasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class KnightGenerator {

public:
    // Constructor
    KnightGenerator(
        board::BitBoards& bitboards,
        board::GameStateBitMasks& gameStateBitmasks,
        int& moveIndex,
        CommonLogic* commonLogic
    );
    
    // Public member functions
    void generate(bool isWhite, std::vector<move::Move>& moveList);

private:
    // Private member functions
    board::BitBoards& _bitboards;
    board::GameStateBitMasks& _gameStateBitmasks;
    int& _moveIndex;
    CommonLogic* _commonLogic;

    // Memory
    std::vector<int> _knightIndices;
    std::vector<U64> _knightBitmasks;
    std::vector<int> _freeMovesIndices;
    std::vector<int> _capturableMovesIndices;
};

}