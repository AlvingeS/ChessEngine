#include "headers/board_printer.h"
#include <iostream>

namespace game {
    BoardPrinter::BoardPrinter(std::unordered_map<PieceType, utils::U64> bitboards) {
        fill_board(bitboards);
    }

    char BoardPrinter::piece_type_to_char(PieceType piece_type) {
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
            case PieceType::W_ALL: return 'x';
            case PieceType::B_ALL: return 'y';
            case PieceType::OCCUPIED: return 'o';
            case PieceType::EMPTY: return 'e';
        }

        return ' ';
    }

    bool BoardPrinter::is_piece_bitboard(PieceType piece_type) {
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

    void BoardPrinter::fill_board(std::unordered_map<PieceType, utils::U64> bitboards) {
        m_board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
        
        for (auto& kvp : bitboards) {
            if (!is_piece_bitboard(kvp.first)) {
                continue;
            }

            char piece_char = piece_type_to_char(kvp.first);
            utils::U64 bitboard = kvp.second;

            for (int i = 0; i < 64; i++) {
                if (utils::get_bit(bitboard, i)) {
                    int row = i / 8;
                    int col = i % 8;
                    m_board[row][col] = piece_char;
                }
            }
        }
    }

    void BoardPrinter::print_board() {
        for (int i = 7; i >= 0; i--) {  // Start from the bottom row (row 7) and go upwards.
            std::cout << "----------------------------------" << std::endl;
            for (int j = 7; j >= 0; j--) {  // Start from the rightmost column (column 7) and go leftwards.
                std::cout << "| " << m_board[i][j] << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "----------------------------------" << std::endl;
    }

}