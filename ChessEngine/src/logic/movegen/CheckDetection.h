#pragma once

#include "common.h"

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class CheckDetection {

public:
    CheckDetection(model::Board& board);
    bool isInCheck(bool isWhite) const;

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
    std::vector<bitmask> _knightBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic