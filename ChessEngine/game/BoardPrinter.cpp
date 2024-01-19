#include "BoardPrinter.h"
#include <iostream>

namespace game {
    BoardPrinter::BoardPrinter(std::unordered_map<PieceType, bits::U64> bitboards) {
        fillBoard(bitboards);
    }

    bool BoardPrinter::isValidPiece(PieceType piece_type) {
        switch(piece_type) {
            case PieceType::W_PAWN:
            case PieceType::W_KNIGHT:
            case PieceType::W_BISHOP:
            case PieceType::W_ROOK:
            case PieceType::W_QUEEN:
            case PieceType::W_KING:
            case PieceType::B_PAWN:
            case PieceType::B_KNIGHT:
            case PieceType::B_BISHOP:
            case PieceType::B_ROOK:
            case PieceType::B_QUEEN:
            case PieceType::B_KING:
                return true;
            default:
                return false;
        }
    }

    void BoardPrinter::fillBoard(std::unordered_map<PieceType, bits::U64> bitboards) {
        _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
        
        for (auto& kvp : bitboards) {
            if (!isValidPiece(kvp.first)) {
                continue;
            }

            char piece_char = piece_type_to_char(kvp.first);
            bits::U64 bitboard = kvp.second;

            for (int i = 0; i < 64; i++) {
                if (bits::getBit(bitboard, i)) {
                    int row = i / 8;
                    int col = i % 8;
                    _board[row][col] = piece_char;
                }
            }
        }
    }

    void BoardPrinter::printBoard() {
        for (int i = 7; i >= 0; i--) {  // Start from the bottom row (row 7) and go upwards.
            std::cout << "----------------------------------" << std::endl;
            for (int j = 7; j >= 0; j--) {  // Start from the rightmost column (column 7) and go leftwards.
                std::cout << "| " << _board[i][j] << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "----------------------------------" << std::endl;
    }

}