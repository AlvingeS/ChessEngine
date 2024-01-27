#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("8/1n1n4/5R2/2N5/4P3/8/5p2/7N");
    moveGenerator.genMoves(true);
    std::cout << moveGenerator.getNumLegalMoves() << std::endl;
    
    game::ChessBoard chessBoard = game::ChessBoard();
    chessBoard.setBoardFromFen("8/1n1n4/5R2/2N5/4P3/8/5p2/7N");
    game::BoardPrinter boardPrinter = game::BoardPrinter(chessBoard.getBitboards());
    boardPrinter.printBoard();
    // unsigned long long int a = 0x0000000000FF0501ULL;
    //bits::printU64InBits(a & (a - 1));
    //bits::printU64InBits(1ULL >> 63);
    return 0;
}
