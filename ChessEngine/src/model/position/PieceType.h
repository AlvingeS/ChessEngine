#pragma once
#include "common.h"

namespace model {

struct Piece {
    enum class Type : int {
        W_PAWN,
        W_KNIGHT,
        W_BISHOP,
        W_ROOK,
        W_QUEEN,
        W_KING,
        B_PAWN,
        B_KNIGHT,
        B_BISHOP,
        B_ROOK,
        B_QUEEN,
        B_KING,
        EMPTY,
        ERROR
    };

    static Type getTypeFromChar(char c);
    static char getCharFromType(Type type);
    static int getIntFromType(Type type);
    static Type getTypeFromInt(int integer);
};

} // namespace model

#include "model/position/PieceType.inl"