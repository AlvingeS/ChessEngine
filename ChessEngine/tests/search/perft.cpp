#include "ChessEngine/search/Searcher.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>

namespace search {
    class perft : public ::testing::Test {
        protected:
            Searcher searcher;
            bool longRun = false;
            std::string pos2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R";
            std::string pos3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8";

        perft() : searcher(5) {}
    };

    TEST_F(perft, perft_starting_pos) {
        searcher.setMaxDepth(longRun ? 6 : 5);
        searcher.minimax(0, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 20},
            {2, 400},
            {3, 8902},
            {4, 197281},
            {5, 4865609},
            {6, 119060324}
        };

        std::unordered_map<int, int> exepectedCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 34},
            {4, 1576},
            {5, 82719},
            {6, 2812008}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 258},
            {6, 5248}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 12},
            {4, 469},
            {5, 27351},
            {6, 809099}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 8},
            {5, 347},
            {6, 0} // Cant check for checkmate at max depth
        };

        for (int i = 1; i <= searcher.getMaxDepth(); i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            ASSERT_EQ(searcher._captureCount[i], exepectedCaptures[i]);
            ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
            ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
            ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
            ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);
            
            if (i < searcher.getMaxDepth()) {
                ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
            }
        }
    }

    TEST_F(perft, perft_pos2) {
        searcher.setMaxDepth(longRun ? 5 : 4);
        searcher.setBoardFromFen(pos2);
        searcher.minimax(0, true, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 48},
            {2, 2039},
            {3, 97862},
            {4, 4085603},
            {5, 193690690}
        };

        std::unordered_map<int, int> expectedCaptures = {
            {0, 0},
            {1, 8},
            {2, 351},
            {3, 17102},
            {4, 757163},
            {5, 35043416}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 1},
            {3, 45},
            {4, 1929},
            {5, 73365}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 2},
            {2, 91},
            {3, 3162},
            {4, 128013},
            {5, 4993637}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 15172},
            {5, 8392}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 0},
            {2, 3},
            {3, 993},
            {4, 25523},
            {5, 3309887}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 1},
            {4, 43},
            {5, 0} // Cant check for checkmate at max depth
        };

        for (int i = 1; i <= searcher.getMaxDepth() - 1; i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            ASSERT_EQ(searcher._captureCount[i], expectedCaptures[i]);
            ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
            ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
            ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
            ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);
            
            if (i < searcher.getMaxDepth()) {
                ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
            }        }
    }

    TEST_F(perft, perft_pos3) {
        searcher.setMaxDepth(longRun ? 6 : 5);
        searcher.setBoardFromFen(pos3);
        searcher.minimax(0, true, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 14},
            {2, 191},
            {3, 2812},
            {4, 43238},
            {5, 674624},
            {6, 11030083},
            {7, 178633661}
        };

        std::unordered_map<int, int> expectedCaptures = {
            {0, 0},
            {1, 1},
            {2, 14},
            {3, 209},
            {4, 3348},
            {5, 52051},
            {6, 940350},
            {7, 14519036}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 2},
            {4, 123},
            {5, 1165},
            {6, 33325},
            {7, 294874}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0},
            {7, 0}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 7552},
            {7, 140024}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 2},
            {2, 10},
            {3, 267},
            {4, 1680},
            {5, 52950},
            {6, 452473},
            {7, 12797406}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 17},
            {5, 0},
            {6, 2733},
            {7, 87}
        };

        for (int i = 1; i <= searcher.getMaxDepth(); i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            ASSERT_EQ(searcher._captureCount[i], expectedCaptures[i]);
            ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
            ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
            ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
            ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);

            if (i < searcher.getMaxDepth()) {
                ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
            }
        }
    }
}

