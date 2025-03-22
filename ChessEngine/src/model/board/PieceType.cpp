#include "ChessEngine/src/model/board/PieceType.h"

namespace model {

Piece::Type Piece::getTypeFromChar(char c) {
    switch (c) {
        case 'P': return Piece::Type::W_PAWN;
        case 'N': return Piece::Type::W_KNIGHT;
        case 'B': return Piece::Type::W_BISHOP;
        case 'R': return Piece::Type::W_ROOK;
        case 'Q': return Piece::Type::W_QUEEN;
        case 'K': return Piece::Type::W_KING;
        case 'p': return Piece::Type::B_PAWN;
        case 'n': return Piece::Type::B_KNIGHT;
        case 'b': return Piece::Type::B_BISHOP;
        case 'r': return Piece::Type::B_ROOK;
        case 'q': return Piece::Type::B_QUEEN;
        case 'k': return Piece::Type::B_KING;
        default : return Piece::Type::ERROR;
    }
}

char Piece::getCharFromType(Piece::Type type) {
    switch(type) {
        case Piece::Type::W_PAWN: return 'P';
        case Piece::Type::W_KNIGHT: return 'N';
        case Piece::Type::W_BISHOP: return 'B';
        case Piece::Type::W_ROOK: return 'R';
        case Piece::Type::W_QUEEN: return 'Q';
        case Piece::Type::W_KING: return 'K';
        case Piece::Type::B_PAWN: return 'p';
        case Piece::Type::B_KNIGHT: return 'n';
        case Piece::Type::B_BISHOP: return 'b';
        case Piece::Type::B_ROOK: return 'r';
        case Piece::Type::B_QUEEN: return 'q';
        case Piece::Type::B_KING: return 'k';
        default : return 'e';
    }
}

} // namespace model