#include "ChessEngine/game/headers/chessboard.h"
#include "ChessEngine/game/headers/board_printer.h"
#include <iostream>

int main() {
    game::ChessBoard chessboard = game::ChessBoard();
    chessboard.set_board_from_fen("p7/P1b5/2B1n3/4N1r1/6R1/8/k6q/K6Q");
    game::BoardPrinter board_printer = game::BoardPrinter(chessboard.get_bitboards());
    board_printer.print_board();

    std::cout << "Done!" << std::endl;
    return 0;
}
