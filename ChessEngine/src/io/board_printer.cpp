#include "io/board_printer.h"

#include "logic/utils.h"

#include <iostream>
#include <cstdio>

namespace io {

BoardPrinter::BoardPrinter(const model::Position& pos)
    : pos_{pos}
{
    fill_boards();
}

void BoardPrinter::fill_boards() 
{
    board_bbs_ = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        model::Piece::Type piece_type = model::Piece::get_type_from_int(i);
        bitboard bb = pos_.bbs.get_bb_from_idx(i);
        char piece_char = model::Piece::get_char_from_type(piece_type);

        for (sq_t sq = 0; sq < 64; sq++) {
            if ((bb >> sq) & 1) {
                int row = sq / 8;
                int col = sq % 8;
                board_bbs_[row][col] = piece_char;
            }
        }
    }

    board_pm_ = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (sq_t sq = 0; sq < 64; sq++) {
        int row = sq / 8;
        int col = sq % 8;
        board_pm_[row][col] = model::Piece::get_char_from_type(pos_.piece_map.get_piece_type_at(sq));
    }
}

void BoardPrinter::print(std::optional<model::Move> move)
{
    if (move.has_value()) {
        int from_col = logic::utils::file_from_sq(move.value().from());
        int from_row = logic::utils::rank_from_sq(move.value().from());
        board_bbs_[from_row][from_col] = '*';
        board_pm_[from_row][from_col]  = '*';
    }
    
    int to_row = -1;
    int to_col = -1;
    if (move.has_value()) {
        to_col = logic::utils::file_from_sq(move.value().to());
        to_row = logic::utils::rank_from_sq(move.value().to());
    }

    for (int row = 7; row >= 0; row--) {  // Start from the top row (row 7) and go downwards.
        if (row == 7) {
            std::cout << "----------Bitboards--------------   ------------Indices--------------   -----------PieceMap--------------" << std::endl;
        } else {
            std::cout << "---------------------------------   ---------------------------------   ---------------------------------" << std::endl;
        }
        
        for (int col = 0; col < 8; col++) {  // Iterate through columns from left to right.
            char piece = board_bbs_[row][col];
            if (to_row == row && to_col == col) {
                std::cout << "|>" << piece << "<";
            } else {
                std::cout << "| " << piece << " ";  // Print the piece character.
            }
        }

        std::cout << "|   ";  // Separate the two boards.

        for (int col = 0; col < 8; col++) {  // Iterate again for bit idxs.
            int bit_idx = row * 8 + col;
            std::cout << "| " << bit_idx << (bit_idx < 10 ? " " : "");  // Print the bit index, add extra space for single digit numbers.
        }

        std::cout << "|   ";  // Separate the two boards.

        for (int col = 0; col < 8; col++) {  // Iterate through columns from left to right.
            char piece = board_pm_[row][col];
            if (to_row == row && to_col == col) {
                std::cout << "|>" << piece << "<";
            } else {
                std::cout << "| " << piece << " ";  // Print the piece character.
            }
        }

        std::cout << "|" << std::endl;  // End of the row.
    }
    std::cout << "---------------------------------   ---------------------------------   ---------------------------------" << std::endl;
    if (move.has_value()) {
        std::printf("Debug info: occupancy_mask=%lu, move=%d\n", pos_.occ_masks.get_occupied_squares_mask(), move.value().value());
    }
}

} // namespace io