#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"

namespace board {

class ZHasher {

public:
    ZHasher();

private:

    uint64_t _randBoardPieceTypeNums[64][12];
    uint64_t _randCastleRightsNums[4];
    uint64_t _randEnPassantFileNums[8];
    uint64_t _randIsWhiteNum;
    uint64_t _hash;
};

} // namespace board
