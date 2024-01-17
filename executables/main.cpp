#include "ChessEngine/game/headers/chessboard.h"
#include "ChessEngine/game/headers/board_printer.h"
#include "ChessEngine/utils/headers/bit_logic.h"
#include <iostream>

int main() {
    game::ChessBoard chessboard = game::ChessBoard();
    chessboard.set_board_from_fen("8/2P5/8/8/8/b1R5/4q3/8");
    game::BoardPrinter board_printer = game::BoardPrinter(chessboard.get_bitboards());
    board_printer.print_board();
    unsigned long long int a = 0x0000000000000001ULL;
    utils::print_bits(a);
    utils::print_bits(a & (a - 1));

    for (int i : utils::get_bit_indices(a)) {
        std::cout << i << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
