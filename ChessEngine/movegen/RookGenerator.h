#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitmasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class RookGenerator {

public:
    // Constructor
    RookGenerator(board::Bitboards& bitboards, RayLogic* rayLogic);
    
    // Public member functions
    void generate(bool isWhite, std::vector<move::Move>& moveList);

private:
    // Private member functions
    board::Bitboards& _bitboards;
    RayLogic* _rayLogic;

    // Memory
    std::vector<int> _rookIndices;
    std::vector<masks::StraightRays> _straightRayBitmasks;
};

} // namespace movegen