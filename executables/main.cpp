#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessBitTools.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("8/2P5/8/8/8/b1R5/4q3/8");
    moveGenerator.genMoves(true);
    std::cout << moveGenerator.getNumLegalMoves() << std::endl;
    game::ChessBoard chessBoard = game::ChessBoard();
    chessBoard.setBoardFromFen("8/2P5/8/8/8/b1R5/4q3/8");
    game::BoardPrinter boardPrinter = game::BoardPrinter(chessBoard.getBitboards());
    boardPrinter.printBoard();
    unsigned long long int a = 0x0000000000FF0501ULL;
    //bits::printU64InBits(a);
    //bits::printU64InBits(a & (a - 1));
    //bits::printU64InBits(1ULL >> 63);


    std::cout << "Done!" << std::endl;
    return 0;
}
