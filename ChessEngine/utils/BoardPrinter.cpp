#include "BoardPrinter.h"

#include <iostream>

#include "ChessEngine/game/PieceType.h"
#include "ChessEngine/utils/BitBasics.h"

namespace utils {
    BoardPrinter::BoardPrinter(std::vector<U64*> bitboards) {
        fillBoard(bitboards);
    }

    bool BoardPrinter::isValidPiece(game::PieceType piece_type) {
        switch(piece_type) {
            case game::PieceType::W_PAWN:
            case game::PieceType::W_KNIGHT:
            case game::PieceType::W_BISHOP:
            case game::PieceType::W_ROOK:
            case game::PieceType::W_QUEEN:
            case game::PieceType::W_KING:
            case game::PieceType::B_PAWN:
            case game::PieceType::B_KNIGHT:
            case game::PieceType::B_BISHOP:
            case game::PieceType::B_ROOK:
            case game::PieceType::B_QUEEN:
            case game::PieceType::B_KING:
                return true;
            default:
                return false;
        }
    }

    void BoardPrinter::fillBoard(std::vector<U64*> bitboards) {
        _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
        
        for (int i = 0; i < 12; i++) {
            game::PieceType pieceType = game::intToPieceType(i);
            U64 bitboard = *(bitboards[i]);
            char pieceChar = game::pieceTypeToChar(pieceType);

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


}