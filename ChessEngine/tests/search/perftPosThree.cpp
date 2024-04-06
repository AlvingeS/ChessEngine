#include "perftBase.h"

namespace search {
    class perftPosThree : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }

        // Nodes, captures, epCaptures, castling, promotion, check, checkmate
        std::unordered_map<int, std::vector<int>> expectedResults {
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
            int depth = longRuns ? posThreeMaxDepth + 1 : posThreeMaxDepth;
            std::unordered_map<std::string, int> stockfishResults = getStockFishPerftResults(posThree, depth);

            searcher.setMaxDepth(depth);
            searcher.setBoardFromFen(posThree);
            bool whiteToStart = true;

            searcher.minimax(0, whiteToStart, 0);

            std::unordered_map<std::string, int> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
            compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
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