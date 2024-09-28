#include "perftBase_test.h"

namespace perft {
    class perftStartPos : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }
    };


    TEST_F(perftStartPos, perft_starting_pos) {
        if (enableStartPosTest) {
            int depth = longRuns ? startPosMaxDepth + 1 : startPosMaxDepth;
            if (megaRuns) {
                depth = 7;
                std::cout << "Running mega run for starting position" << std::endl;
            }

            std::unordered_map<std::string, long> stockfishResults = getStockFishPerftResults(startPos, depth);

            searcher.setMaxDepth(depth);
            bool whiteToStart = true;

            searcher.minimax(0, whiteToStart, 0);
            std::unordered_map<std::string, long> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
            compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

            std::unordered_map<int, std::vector<long>> expectedResults {
                {0, {1,         0,          0,    0, 0, 0,      0}},
                {1, {20,        0,          0,    0, 0, 0,      0}},
                {2, {400,       0,          0,    0, 0, 0,      0}},
                {3, {8902,      34,         0,    0, 0, 12,     0}},
                {4, {197281,    1576,       0,    0, 0, 469,    8}},
                {5, {4865609,   82719,      258,  0, 0, 27351,  347}},
                {6, {119060324, 2812008,    5248, 0, 0, 809099, 10828}},
                {7, {3195901860, 108329926,  319617, 883453, 0, 33103848, 0}}
            };

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

