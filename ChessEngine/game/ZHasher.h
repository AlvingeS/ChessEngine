#pragma once

#include "PieceType.h"

namespace game {
    class ZHasher {
        public:
            ZHasher();

        private:
            unsigned long long _randBoardPieceTypeNums[64][12];
            unsigned long long _randCastleRightsNums[4];
            unsigned long long _randEnPassantFileNums[8];
            unsigned long long _randIsWhiteNum;
            unsigned long long _hash;
    };
}