#include "ChessEngine/src/utils/BoardPrinter.h"

#include "ChessEngine/src/utils/BitBasics.h"

#include <iostream>

namespace utils {

BoardPrinter::BoardPrinter(const game::board::Bitboards& bitboards) 
{
    fillBoard(bitboards);
}

bool BoardPrinter::isValidPiece(game::board::PieceType piece_type) 
{
    switch(piece_type) {
        case game::board::PieceType::W_PAWN:
        case game::board::PieceType::W_KNIGHT:
        case game::board::PieceType::W_BISHOP:
        case game::board::PieceType::W_ROOK:
        case game::board::PieceType::W_QUEEN:
        case game::board::PieceType::W_KING:
        case game::board::PieceType::B_PAWN:
        case game::board::PieceType::B_KNIGHT:
        case game::board::PieceType::B_BISHOP:
        case game::board::PieceType::B_ROOK:
        case game::board::PieceType::B_QUEEN:
        case game::board::PieceType::B_KING:
            return true;
        default:
            return false;
    }
}

void BoardPrinter::fillBoard(const game::board::Bitboards& bitboards) 
{
    _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        game::board::PieceType pieceType = game::board::intToPieceType(i);
        bitmask bitboard = bitboards.getBitboardFromIndex(i);
        char pieceChar = game::board::pieceTypeToChar(pieceType);

        for (int i = 0; i < 64; i++) {
            if (utils::getBit(bitboard, i)) {
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

} // namespace utils