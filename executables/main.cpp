#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("rnbqkb1r/pppp1ppp/5n2/3Pp3/8/8/8/RNBQKBNR");
    game::BoardPrinter boardPrinter = game::BoardPrinter(moveGenerator.getBoard().getBitboards());
    boardPrinter.printBoard();

    for (int i = 0; i < 100000 - 1; i++) {
        moveGenerator.genMoves(true);
        moveGenerator.resetMoves();
    }

    moveGenerator.genMoves(true);

    std::cout << "Number of legal moves: " << moveGenerator.getMoveIndex() << std::endl;
    
    return 0;
}
