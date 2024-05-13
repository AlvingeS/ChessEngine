#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/BoardPrinter.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/movegen/MoveGenerator.h"
#include <iostream>
#include "ChessEngine/search/Searcher.h"
#include <chrono>

int main() {

    /*
    Total number of nodes for depth 4: 6605806

    Changelog:
        - 2024-04-08: 
            First unoptimized runs: 
            3.803s ~ 1.736M nodes/s ~ 0.7% SF

            Added option to disable perft stats recording, will not use from now on
            2.937s ~ 2,249M nodes/s ~ 0.9% SF

            Removed unnecessary copying of bitboard in make/unmake move
            2.881s ~ 2,345M nodes/s ~ 0.9% SF

            Now handle white/black bitmasks directly in make/unmake move
            2.603s ~ 2,567M nodes/s ~ 1.0% SF
        - 2024-04-10:
            Removed unnecessary vector copying in pawn-move generation
            2.469s ~ 2,678M nodes/s ~ 1.1% SF

            Changed the ordering of the move flags to check for captures,
            promotions, or castles in groups
            2.139s ~ 3,118M nodes/s ~ 1.2% SF

            Made getBitBoard inline
            2.122s ~ 3,121M nodes/s ~ 1.2% SF

        - 2024-04-14
            Completed refactoring of movegeneration, don't know what caused slight speedup
            2.063s ~ 3.300M nodes/s ~ 1.3% SF

        - 2024-05-08
            Refactored acessing bitboards to use pointers directly instead of lookup
            1.940s ~ 3.471M nodes/s ~ 1.4% SF

            Removed castling logic that I had forgotten about, it is all handled by the searcher
            1.783s ~ 3.686M nodes/s ~ 1.5% SF

        - 2024-05-12
            !!! SWITCHED TO RELEASE MODE FOR PERFORMANCE TESTING !!!
            Previous commit had the following stats in release mode
            0.341s ~ 19.291M nodes/s ~ 7.8% SF

            Refactored Chessboard into an additional movemaker class, however
            performance has dropped so this will be worked on in the future
            0.421s ~ 16.102M nodes/s ~ 6.4% SF

            Moved castling flags to last so I only have to check if flag is
            greater than 14 to see if it is a castle
            0.346s ~ 19.145M nodes/s ~ 7.8% SF

            Refactored searcher so that castling rights and last captured pieces info
            is now in its on class, will add everything else as well
            0.337s ~ 20.009M nodes/s ~ 8.0% SF
    */

    // Start clock
    auto start = std::chrono::high_resolution_clock::now();

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

    // End clock
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    float nodesPerSecond = 6605806 / elapsed_seconds.count();
    std::cout << "Nodes per second: " << nodesPerSecond << std::endl;
    std::cout << "SF%: " << nodesPerSecond / 250000000 << std::endl;

    return 0;
}
