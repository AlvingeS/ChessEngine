#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/KingBitmasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class KingGenerator {

public:
    // Constructor
    KingGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks,
        int& moveIndex,
        CommonLogic* commonLogic
    );
    
    // Public member functions
    void generate(bool isWhite, std::vector<move::Move>& moveList);

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;
    int& _moveIndex;
    CommonLogic* _commonLogic;

    // Memory
    std::vector<int> _kingIndices;
    std::vector<bitmask> _kingBitmasks;
    std::vector<int> _freeMovesIndices;
    std::vector<int> _capturableMovesIndices;
};

} // namespace movegen