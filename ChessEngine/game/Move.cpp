#include "Move.h"

namespace game {
    Move::Move() {
        _pieceType = PieceType::ERROR;
        _bitIndexFrom = -1;
        _bitIndexTo = -1;
    }

    Move::Move(PieceType pieceType, int bitIndexFrom, int bitIndexTo) {
        _pieceType = pieceType;
        _bitIndexFrom = bitIndexFrom;
        _bitIndexTo = bitIndexTo;
    }

    PieceType Move::getPieceType() {
        return _pieceType;
    }
}