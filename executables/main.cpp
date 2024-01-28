#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("5r2/3qk3/4R3/8/8/8/Pn6/1K6");
    moveGenerator.genMoves(true);
    std::cout << moveGenerator.getNumLegalMoves() << std::endl;
    
    game::ChessBoard chessBoard = game::ChessBoard();
    chessBoard.setBoardFromFen("5r2/3qk3/4R3/8/8/8/Pn6/1K6");
    game::BoardPrinter boardPrinter = game::BoardPrinter(chessBoard.getBitboards());
    boardPrinter.printBoard();
    // unsigned long long int a = 0x0000000000FF0501ULL;
    //bits::printU64InBits(a & (a - 1));
    //bits::printU64InBits(1ULL >> 63);
    return 0;
}
