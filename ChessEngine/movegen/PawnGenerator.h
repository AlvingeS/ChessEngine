#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/PawnBitmasks.h"
#include "ChessEngine/perft/SearchMemory.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class PawnGenerator {

public:
    // Constructor
    PawnGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks,
        int& moveIndex,
        CommonLogic* commonLogic
    );
    
    // Public member functions
    void generate(
        bool isWhite,
        std::vector<move::Move>& moveList,
        int currentDepth,
        perft::SearchMemory& searchMemory
    );

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;
    int& _moveIndex;
    CommonLogic* _commonLogic;

    // Memory
    std::vector<int> _pawnIndices;
    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;

    std::vector<int> _freeMovesIndices;
    std::vector<int> _capturableMovesIndices;
};

}