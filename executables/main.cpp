#include "ChessEngine/game/headers/chessboard.h"
#include "ChessEngine/game/headers/board_printer.h"
#include <iostream>

int main() {
    game::ChessBoard chessboard = game::ChessBoard();
    chessboard.set_board_from_fen("8/2P5/8/8/8/b1R5/4q3/8");
    game::BoardPrinter board_printer = game::BoardPrinter(chessboard.get_bitboards());
    board_printer.print_board();

    std::cout << "Done!" << std::endl;
    return 0;
}
