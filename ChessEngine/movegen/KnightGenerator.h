#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/KnightBitmasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class KnightGenerator {

public:
    // Constructor
    KnightGenerator(
        board::Bitboards& bitboards,
        board::GameStateBitmasks& gameStateBitmasks,
        int& moveIndex,
        CommonLogic* commonLogic
    );
    
    // Public member functions
    void generate(bool isWhite, std::vector<move::Move>& moveList);

private:
    // Private member functions
    board::Bitboards& _bitboards;
    board::GameStateBitmasks& _gameStateBitmasks;
    int& _moveIndex;
    CommonLogic* _commonLogic;

    // Memory
    std::vector<int> _knightIndices;
    std::vector<bitmask> _knightBitmasks;
    std::vector<int> _freeMovesIndices;
    std::vector<int> _capturableMovesIndices;
};

}