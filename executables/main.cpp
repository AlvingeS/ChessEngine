#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("7r/8/8/1P6/8/1R1p3R/8/8");
    moveGenerator.genMoves(true);
    std::cout << moveGenerator.getNumLegalMoves() << std::endl;
    
    game::ChessBoard chessBoard = game::ChessBoard();
    chessBoard.setBoardFromFen("7r/8/8/1P6/8/1R1p3R/8/8");
    game::BoardPrinter boardPrinter = game::BoardPrinter(chessBoard.getBitboards());
    boardPrinter.printBoard();
    unsigned long long int a = 0x0000000000FF0501ULL;
    bits::printU64InBits(a);
    a |= (1ULL << 100);
    bits::printU64InBits(a);
    //bits::printU64InBits(a & (a - 1));
    //bits::printU64InBits(1ULL >> 63);
    return 0;
}
