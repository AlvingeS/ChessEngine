#include "ChessEngine/search/Searcher.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include "perftBase.h"

namespace search {
    class perftStartPos : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }
    };


    TEST_F(perftStartPos, perft_starting_pos) {
        searcher.setMaxDepth(longRuns ? 6 : 5);
        
        if (enableStartPosTest) {
            searcher.minimax(0, true, 0);
        }

        std::unordered_map<int, std::vector<int>> expectedResults {
            {0, {1,         0,          0,          0,      0,          0,          0}},
            {1, {20,        0,          0,          0,      0,          0,          0}},
            {2, {400,       0,          0,          0,      0,          0,          0}},
            {3, {8902,      34,         0,          0,      0,          12,         0}},
            {4, {197281,    1576,       0,          0,      0,          469,        8}},
            {5, {4865609,   82719,      258,        0,      0,          27351,      347}},
            {6, {119060324, 2812008,    5248,       0,      0,          809099,     0}}
        };

        if (enableStartPosTest) {
            for (int i = 1; i <= searcher.getMaxDepth() - 1; i++) {
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

