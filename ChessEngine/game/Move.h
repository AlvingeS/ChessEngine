#pragma once
#include "PieceType.h"

namespace game {
    class Move {
        public:
            Move();
            Move(PieceType pieceType, int bitIndexFrom, int bitIndexTo);
            PieceType getPieceType();

        private:
            PieceType _pieceType;
            int _bitIndexFrom;
            int _bitIndexTo;
    };
}
