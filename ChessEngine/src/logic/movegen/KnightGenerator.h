#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"

namespace logic {

class KnightGenerator {

public:
    KnightGenerator(model::Board& board);
    void generate(bool isWhite, model::Movelist& moveList);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<bitmask> _knightBitmasks;
};

} // namespace logic