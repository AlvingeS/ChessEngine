#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"

namespace board {
    class ZHasher {
        public:
            ZHasher();

        private:
            U64 _randBoardPieceTypeNums[64][12];
            U64 _randCastleRightsNums[4];
            U64 _randEnPassantFileNums[8];
            U64 _randIsWhiteNum;
            U64 _hash;
    };
}