#include "ChessEngine/src/io/BoardPrinter.h"

#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

#include <iostream>

namespace io {

BoardPrinter::BoardPrinter(const model::Bitboards& bitboards) 
{
    fillBoard(bitboards);
}

bool BoardPrinter::isValidPiece(model::PieceType piece_type) 
{
    switch(piece_type) {
        case model::PieceType::W_PAWN:
        case model::PieceType::W_KNIGHT:
        case model::PieceType::W_BISHOP:
        case model::PieceType::W_ROOK:
        case model::PieceType::W_QUEEN:
        case model::PieceType::W_KING:
        case model::PieceType::B_PAWN:
        case model::PieceType::B_KNIGHT:
        case model::PieceType::B_BISHOP:
        case model::PieceType::B_ROOK:
        case model::PieceType::B_QUEEN:
        case model::PieceType::B_KING:
            return true;
        default:
            return false;
    }
}

void BoardPrinter::fillBoard(const model::Bitboards& bitboards) 
{
    _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        model::PieceType pieceType = model::intToPieceType(i);
        bitmask bitboard = bitboards.getBitboardFromIndex(i);
        char pieceChar = model::pieceTypeToChar(pieceType);

        for (int i = 0; i < 64; i++) {
            if ((bitboard >> i) & 1) {
                int row = i / 8;
                int col = i % 8;
                _board[row][col] = pieceChar;
            }
        }
    }
}

void BoardPrinter::printBoard() const {
    for (int row = 7; row >= 0; row--) {  // Start from the bottom row (row 7) and go upwards.
        std::cout << "---------------------------------   ---------------------------------" << std::endl;
        for (int col = 7; col >= 0; col--) {  // Iterate through columns from left to right.
            char piece = _board[row][col];
            std::cout << "| " << piece << " ";  // Print the piece character.
        }

        std::cout << "|   ";  // Separate the two boards.

        for (int col = 7; col >= 0; col--) {  // Iterate again for bit indices.
            int bitIndex = row * 8 + col;
            std::cout << "| " << bitIndex << (bitIndex < 10 ? " " : "");  // Print the bit index, add extra space for single digit numbers.
        }

        std::cout << "|" << std::endl;  // End of the row.
    }
    std::cout << "----------------------------------  ----------------------------------" << std::endl;
}

} // namespace io