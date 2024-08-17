#include "BoardPrinter.h"

#include <iostream>

#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/utils/BitBasics.h"

namespace utils {

BoardPrinter::BoardPrinter(const board::Bitboards& bitboards) 
{
    fillBoard(bitboards);
}

bool BoardPrinter::isValidPiece(board::PieceType piece_type) 
{
    switch(piece_type) {
        case board::PieceType::W_PAWN:
        case board::PieceType::W_KNIGHT:
        case board::PieceType::W_BISHOP:
        case board::PieceType::W_ROOK:
        case board::PieceType::W_QUEEN:
        case board::PieceType::W_KING:
        case board::PieceType::B_PAWN:
        case board::PieceType::B_KNIGHT:
        case board::PieceType::B_BISHOP:
        case board::PieceType::B_ROOK:
        case board::PieceType::B_QUEEN:
        case board::PieceType::B_KING:
            return true;
        default:
            return false;
    }
}

void BoardPrinter::fillBoard(const board::Bitboards& bitboards) 
{
    _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        board::PieceType pieceType = board::intToPieceType(i);
        bitmask bitboard = bitboards.getBitboardFromIndex(i);
        char pieceChar = board::pieceTypeToChar(pieceType);

        for (int i = 0; i < 64; i++) {
            if (utils::getBit(bitboard, i)) {
                int row = i / 8;
                int col = i % 8;
                _board[row][col] = pieceChar;
            }
        }
    }
}

void BoardPrinter::printBoard() {
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

} // namespace utils