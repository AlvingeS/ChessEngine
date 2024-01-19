#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/BitUtils.h"
#include <iostream>

int main() {
    game::ChessBoard chessBoard = game::ChessBoard();
    chessBoard.setBoardFromFen("8/2P5/8/8/8/b1R5/4q3/8");
    game::BoardPrinter boardPrinter = game::BoardPrinter(chessBoard.getBitboards());
    boardPrinter.printBoard();
    unsigned long long int a = 0x0000000000000001ULL;
    bits::printU64InBits(a);
    bits::printU64InBits(a & (a - 1));

    for (int i : bits::getBitIndices(a)) {
        std::cout << i << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
