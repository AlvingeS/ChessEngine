#include "ChessEngine/src/representation/board/PieceType.h"

namespace representation {
namespace board {

PieceType CharToPieceType(char c) 
{
    switch (c) {
        case 'P': return PieceType::W_PAWN;
        case 'N': return PieceType::W_KNIGHT;
        case 'B': return PieceType::W_BISHOP;
        case 'R': return PieceType::W_ROOK;
        case 'Q': return PieceType::W_QUEEN;
        case 'K': return PieceType::W_KING;
        case 'p': return PieceType::B_PAWN;
        case 'n': return PieceType::B_KNIGHT;
        case 'b': return PieceType::B_BISHOP;
        case 'r': return PieceType::B_ROOK;
        case 'q': return PieceType::B_QUEEN;
        case 'k': return PieceType::B_KING;
        default : return PieceType::ERROR;
    }
}

char pieceTypeToChar(PieceType piece_type) 
{
    switch(piece_type) {
        case PieceType::W_PAWN: return 'P';
        case PieceType::W_KNIGHT: return 'N';
        case PieceType::W_BISHOP: return 'B';
        case PieceType::W_ROOK: return 'R';
        case PieceType::W_QUEEN: return 'Q';
        case PieceType::W_KING: return 'K';
        case PieceType::B_PAWN: return 'p';
        case PieceType::B_KNIGHT: return 'n';
        case PieceType::B_BISHOP: return 'b';
        case PieceType::B_ROOK: return 'r';
        case PieceType::B_QUEEN: return 'q';
        case PieceType::B_KING: return 'k';
        default : return 'e';
    }
}

int pieceTypeToInt(PieceType piece_type) 
{
    return static_cast<int>(piece_type);
}

PieceType intToPieceType(int i) 
{
    return static_cast<PieceType>(i);
}

} // namespace board
} // namespace representation