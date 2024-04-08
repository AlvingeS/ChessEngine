#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/MoveGenerator.h"
#include <iostream>
#include "ChessEngine/search/Searcher.h"

int main() {

    /*
    Total number of nodes for depth 4: 6605806

    Changelog:
        - 2024-04-08: 
            First unoptimized runs: 
            3.803s ~ 1.736M nodes/s ~ 0.7% SF

            Added option to disable perft stats recording, will not use from now on
            2.937s ~ 2,249M nodes/s ~ 0.9% SF
    
    */

    int const MAX_DEPTH = 4;

    std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string posTwo = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
    std::string posThree = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
    std::string posFive = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    
    bool recPerftStats = false;

    search::Searcher searcherStartPos = search::Searcher(MAX_DEPTH);
    searcherStartPos.setBoardFromFen(startPos);
    searcherStartPos.minimax(0, true, 0, recPerftStats);

    search::Searcher searcherPosTwo = search::Searcher(MAX_DEPTH);
    searcherPosTwo.setBoardFromFen(posTwo);
    searcherPosTwo.minimax(0, true, 0, recPerftStats);

    search::Searcher searcherPosThree = search::Searcher(MAX_DEPTH);
    searcherPosThree.setBoardFromFen(posThree);
    searcherPosThree.minimax(0, true, 0, recPerftStats);

    search::Searcher searcherPosFive = search::Searcher(MAX_DEPTH);
    searcherPosFive.setBoardFromFen(posFive);
    searcherPosFive.minimax(0, true, 0, recPerftStats);

    if (recPerftStats) {
        int sum = 0;
        sum += searcherStartPos.sumNodesToDepth(MAX_DEPTH);
        sum += searcherPosTwo.sumNodesToDepth(MAX_DEPTH);
        sum += searcherPosThree.sumNodesToDepth(MAX_DEPTH);
        sum += searcherPosFive.sumNodesToDepth(MAX_DEPTH);
        std::cout << "Total nodes: " << sum << std::endl;
    }

    return 0;
}
