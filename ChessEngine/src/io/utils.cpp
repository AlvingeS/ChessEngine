#include "io/utils.h"

namespace io::utils
{

sq_t sq_from_notation(const std::string& notation)
{
    char file = notation[0];
    int row_idx = (notation[1] - '0') - 1;

    int file_idx;
    switch (file) {
        case 'a': file_idx = 0; break;
        case 'b': file_idx = 1; break;
        case 'c': file_idx = 2; break;
        case 'd': file_idx = 3; break;
        case 'e': file_idx = 4; break;
        case 'f': file_idx = 5; break;
        case 'g': file_idx = 6; break;
        case 'h': file_idx = 7; break;
        default:  file_idx = -1;
    }

    return 8 * row_idx + file_idx;
}

PieceType get_type_from_char(char c) {
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
        default : return PieceType::EMPTY;
    }
}

char get_char_from_type(PieceType type) {
    switch(type) {
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
        default : return ' ';
    }
}

} // namespace utils