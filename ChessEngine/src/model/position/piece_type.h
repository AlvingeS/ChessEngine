#pragma once

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

    static Type get_type_from_char(char c);
    static char get_char_from_type(Type type);
    static int get_int_from_type(Type type);
    static Type get_type_from_int(int integer);
};

} // namespace model

#include "model/position/piece_type.inl"