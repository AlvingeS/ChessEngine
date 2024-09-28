#pragma once

#include "ChessEngine/src/common.h"

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

/// @brief Converts a character to a PieceType for FEN parsing
PieceType CharToPieceType(char c);

/// @brief Converts a PieceType to a character for FEN parsing
char pieceTypeToChar(PieceType piece_type); 

/// @brief Converts a PieceType to an integer for indexing purposes
int pieceTypeToInt(PieceType piece_type); 

/// @brief Converts an integer to a PieceType for indexing purposes
PieceType intToPieceType(int i); 

} // namespace board