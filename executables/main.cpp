#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>

int main() {
    game::MoveGenerator moveGenerator = game::MoveGenerator();
    moveGenerator.setBoardFromFen("1k6/R2n2p1/1p2NP2/8/1Q1Bq3/2b5/1P4r1/5K2");

    for (int i = 0; i < 100000 - 1; i++) {
        moveGenerator.genMoves(true);
        moveGenerator.resetMoves();
    }

    moveGenerator.genMoves(true);

    std::cout << "Number of legal moves: " << moveGenerator.getMoveIndex() << std::endl;
    
    return 0;
}
