#pragma once
#include <cstdint>

namespace game {

    enum class PieceType : uint8_t {
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
        W_ALL,
        B_ALL,
        OCCUPIED,
        EMPTY
    };
}
