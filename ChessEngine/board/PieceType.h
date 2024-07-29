#pragma once

#include "ChessEngine/common.h"

namespace board {

enum class PieceType : int {
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

PieceType CharToPieceType(const char c);
char pieceTypeToChar(const PieceType piece_type); 
int pieceTypeToInt(const PieceType piece_type); 
PieceType intToPieceType(const int i); 

} // namespace board