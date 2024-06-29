#include "perftBase.h"

namespace perft {
    class perftPosThree : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }

        // Nodes, captures, epCaptures, castling, promotion, check, checkmate
        std::unordered_map<int, std::vector<long>> expectedResults {
            {0, {1,         0,          0,          0,      0,          0,          0}},
            {1, {14,        1,          0,          0,      0,          2,          0}},
            {2, {191,       14,         0,          0,      0,          10,         0}},
            {3, {2812,      209,        2,          0,      0,          267,        0}},
            {4, {43238,     3348,       123,        0,      0,          1680,       17}},
            {5, {674624,    52051,      1165,       0,      0,          52950,      0}},
            {6, {11030083,  940350,     33325,      0,      7552,       452473,     2733}},
            {7, {178633661, 14519036,   294874,     0,      140024,     12797406,   87}}
        };
    };

    TEST_F(perftPosThree, perft_pos3) {
        if (enablePos3Test) {
            searcher.setBoardFromFen(posThree);

            // game::Move move = moveFromStrAndFlag("a5a4", 0);
            // searcher.makeMove(move, true);
            // move = moveFromStrAndFlag("h4g4", 0);
            // searcher.makeMove(move, false);
            // move = moveFromStrAndFlag("a4b3", 0);
            // searcher.makeMove(move, true);
            // move = moveFromStrAndFlag("g4f5", 0);
            // searcher.makeMove(move, false);
            // move = moveFromStrAndFlag("b3c4", 0);
            // searcher.makeMove(move, true);
            // move = moveFromStrAndFlag("f5e4", 0);
            // searcher.makeMove(move, false);

            int nDebugMoves = 0;

            int depth = longRuns ? posThreeMaxDepth + 1 : posThreeMaxDepth;
            depth -= nDebugMoves;
            bool whiteToStart = nDebugMoves % 2 == 0;

            std::string debugFen;
            if (nDebugMoves > 0) {
                debugFen = searcher.getFenFromBoard();
                debugFen += whiteToStart ? " w" : " b";
                debugFen += " - -";
            }

            std::unordered_map<std::string, long> stockfishResults = getStockFishPerftResults(nDebugMoves > 0 ? debugFen : posThree, depth);

            searcher.setMaxDepth(depth);
            searcher.minimax(0, whiteToStart, 0);

            std::unordered_map<std::string, long> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
            compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

            if (nDebugMoves == 0) {
                for (long i = 1; i <= searcher.getMaxDepth(); i++) {
                    ASSERT_EQ(searcher._nodeCount[i], expectedResults[i][0]);
                    ASSERT_EQ(searcher._captureCount[i], expectedResults[i][1]);
                    ASSERT_EQ(searcher._epCaptureCount[i], expectedResults[i][2]);
                    ASSERT_EQ(searcher._castlingCount[i], expectedResults[i][3]);
                    ASSERT_EQ(searcher._promotionCount[i], expectedResults[i][4]);
                    ASSERT_EQ(searcher._checkCount[i], expectedResults[i][5]);
                    
                    if (i < searcher.getMaxDepth()) {
                        ASSERT_EQ(searcher._checkmateCount[i], expectedResults[i][6]);
                    }       
                }
            }
        }
    }
}