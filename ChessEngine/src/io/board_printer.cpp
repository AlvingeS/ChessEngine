#include "io/board_printer.h"

#include "logic/movegen/utils/bits.h"

#include <iostream>

namespace io {

BoardPrinter::BoardPrinter(const model::Bitboards& bbs) 
{
    fill_board(bbs);
}

bool BoardPrinter::is_valid_piece(model::Piece::Type piece_type) 
{
    switch(piece_type) {
        case model::Piece::Type::W_PAWN:
        case model::Piece::Type::W_KNIGHT:
        case model::Piece::Type::W_BISHOP:
        case model::Piece::Type::W_ROOK:
        case model::Piece::Type::W_QUEEN:
        case model::Piece::Type::W_KING:
        case model::Piece::Type::B_PAWN:
        case model::Piece::Type::B_KNIGHT:
        case model::Piece::Type::B_BISHOP:
        case model::Piece::Type::B_ROOK:
        case model::Piece::Type::B_QUEEN:
        case model::Piece::Type::B_KING:
            return true;
        default:
            return false;
    }
}

void BoardPrinter::fill_board(const model::Bitboards& bbs) 
{
    board_ = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        model::Piece::Type piece_type = model::Piece::get_type_from_int(i);
        bitboard bb = bbs.get_bb_from_idx(i);
        char piece_char = model::Piece::get_char_from_type(piece_type);

        for (sq_idx sq = 0; sq < 64; sq++) {
            if ((bb >> sq) & 1) {
                int row = sq / 8;
                int col = sq % 8;
                board_[row][col] = piece_char;
            }
        }
    }
}

void BoardPrinter::print() const {
    for (int row = 7; row >= 0; row--) {  // Start from the bottom row (row 7) and go upwards.
        std::cout << "---------------------------------   ---------------------------------" << std::endl;
        for (int col = 7; col >= 0; col--) {  // Iterate through columns from left to right.
            char piece = board_[row][col];
            std::cout << "| " << piece << " ";  // Print the piece character.
        }

        std::cout << "|   ";  // Separate the two boards.

        for (int col = 7; col >= 0; col--) {  // Iterate again for bit idxs.
            int bit_idx = row * 8 + col;
            std::cout << "| " << bit_idx << (bit_idx < 10 ? " " : "");  // Print the bit index, add extra space for single digit numbers.
        }

        std::cout << "|" << std::endl;  // End of the row.
    }
    std::cout << "----------------------------------  ----------------------------------" << std::endl;
}

} // namespace io