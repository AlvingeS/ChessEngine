#include "perftBase.h"

namespace search {
    class perftPosThree : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }

        // Nodes, captures, epCaptures, castling, promotion, check, checkmate
        std::unordered_map<int, std::vector<int>> expectedResults {
            {0, {1,         0,          0,          0,      0,          0}},
            {1, {14,        1,          0,          0,      0,          2}},
            {2, {191,       14,         0,          0,      0,          10}},
            {3, {2812,      209,        2,          0,      0,          267}},
            {4, {43238,     3348,       123,        0,      0,          1680}},
            {5, {674624,    52051,      1165,       0,      0,          52950}},
            {6, {11030083,  940350,     33325,      0,      7552,       452473}},
            {7, {178633661, 14519036,   435767,     0,      140024,     12797406}}
        };
    };

    TEST_F(perftPosThree, perft_pos3) {
        searcher.setMaxDepth(longRuns ? 7 : 6);
        searcher.setBoardFromFen(posThree);
        searcher.getBoard().setRookAMoved(true, true);
        bool whiteToStart = false;

        if (enablePos3Test) {
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