#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>
#include "ChessEngine/search/Searcher.h"

int main() {

    int const MAX_DEPTH = 5;
    search::Searcher searcher = search::Searcher(MAX_DEPTH);
    searcher.minimax(0, true);
    
    for (int i = 0; i < MAX_DEPTH; i++) {
        std::cout << "Depth " << i << ": " << searcher._nodeCount[i] << std::endl;
    }

    // std::string fenOne = "rnb1kbnr/ppp3pp/8/3pppq1/3P4/2N1PN1P/PPP2PP1/R1BQKB1R";
    // fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
    // fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";


    // searcher.getMoveGenerator().setBoardFromFen(fenOne);
    // game::BoardPrinter boardPrinterOne = game::BoardPrinter(searcher.getBoard().getBitboards());
    // boardPrinterOne.printBoard();

    // moveGenerator.setBoardFromFen(fenTwo);
    // game::BoardPrinter boardPrinterTwo = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    // boardPrinterTwo.printBoard();

    // moveGenerator.setBoardFromFen(fenThree);
    // game::BoardPrinter boardPrinterThree = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    // boardPrinterThree.printBoard();

    // for (int i = 0; i < 100000 - 1; i++) {
    //     moveGenerator.genMoves(true);
    //     moveGenerator.resetMoves();
    // }

    // moveGenerator.genMoves(true);

    // std::cout << "Number of legal moves: " << moveGenerator.getMoveIndex() << std::endl;
    
    return 0;
}
