#include "Move.h"

namespace game {
    Move::Move(PieceType pieceType, int bitIndexFrom, int bitIndexTo) {
        _pieceType = pieceType;
        _bitIndexFrom = bitIndexFrom;
        _bitIndexTo = bitIndexTo;
    }
}