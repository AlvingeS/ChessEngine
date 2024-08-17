#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitmasks.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class BishopGenerator {

public:
    // Constructor
    BishopGenerator(const board::Bitboards& bitboards, RayLogic* rayLogic);
    
    // Public member functions
    void generate(bool isWhite, std::vector<move::Move>& moveList);

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;

    RayLogic* _rayLogic;

    // Memory
    std::vector<int> _bishopIndices;
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace movegen