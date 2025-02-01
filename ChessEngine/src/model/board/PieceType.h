#pragma once
#include "ChessEngine/src/common.h"

namespace model {

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

PieceType CharToPieceType(char c);
char pieceTypeToChar(PieceType piece_type); 
int pieceTypeToInt(PieceType piece_type); 
PieceType intToPieceType(int i); 

} // namespace model