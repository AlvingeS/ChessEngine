#pragma once
#include "PieceType.h"

namespace game {
    class Move {
        public:
            Move(PieceType pieceType, int bitIndexFrom, int bitIndexTo);

        private:
            PieceType _pieceType;
            int _bitIndexFrom;
            int _bitIndexTo;
    };
}
