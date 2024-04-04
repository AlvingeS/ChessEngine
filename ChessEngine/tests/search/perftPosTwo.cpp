#include "perftBase.h"

namespace search {
    class perftPosTwo : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }

        // Nodes, captures, epCaptures, castling, promotion, check, checkmate
        std::unordered_map<int, std::vector<int>> expectedResults {
            {0, {1,         0,          0,          0,      0,          0}},
            {1, {48,        8,          2,          0,      0,          0}},
            {2, {2039,      351,        91,         0,      3,          0}},
            {3, {97862,     17102,      3162,       0,      993,        1}},
            {4, {4085603,   757163,     128013,     15172,  25523,      43}},
            {5, {193690690, 35043416,   4993637,    8392,   3309887,    0}},
            {6, {0,         0,          0,          0,      0,          0}}
        };
    };

    TEST_F(perftPosTwo, perft_pos2) {
        searcher.setMaxDepth(longRuns ? 5 : 4);
        searcher.setBoardFromFen(posTwo);
        bool whiteToStart = false;

        if (enablePos2Test) {
            searcher.minimax(0, whiteToStart, 0);
        }

        std::vector<std::string> strVec = nodeCountPerFirstMoveAsStrVec(whiteToStart);

        if (enablePos2Test) {
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