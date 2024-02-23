#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    std::string fenOne;
    std::string fenTwo;
    std::string fenThree;

    fenOne = "3q4/2P3P1/8/8/8/8/1p5p/2N5";
    fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
    fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";

    moveGenerator.setBoardFromFen(fenOne);
    game::BoardPrinter boardPrinterOne = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    boardPrinterOne.printBoard();

    // moveGenerator.setBoardFromFen(fenTwo);
    // game::BoardPrinter boardPrinterTwo = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    // boardPrinterTwo.printBoard();

    // moveGenerator.setBoardFromFen(fenThree);
    // game::BoardPrinter boardPrinterThree = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    // boardPrinterThree.printBoard();

    for (int i = 0; i < 100000 - 1; i++) {
        moveGenerator.genMoves(true);
        moveGenerator.resetMoves();
    }

    moveGenerator.genMoves(true);

    // std::cout << "Number of legal moves: " << moveGenerator.getMoveIndex() << std::endl;
    
    return 0;
}
