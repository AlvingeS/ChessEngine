#include "ChessEngine/search/Searcher.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>

namespace search {
    class perft : public ::testing::Test {
        protected:
            int MAX_DEPTH = 5;
            Searcher searcher;
            std::string pos2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R";
            std::string pos3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8";

            perft() : searcher(Searcher(MAX_DEPTH)) {}
    };

    TEST_F(perft, perft_starting_pos) {
        // searcher.minimax(0, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 20},
            {2, 400},
            {3, 8902},
            {4, 197281},
            {5, 4865609}
        };

        for (int i = 0; i <= MAX_DEPTH; i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
        }
    }

    TEST_F(perft, perft_pos2) {
        searcher.setBoardFromFen(pos2);
        searcher.setMaxDepth(4);
        // searcher.minimax(0, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 48},
            {2, 2039},
            {3, 97862},
            {4, 4085603}
        };

        for (int i = 1; i <= MAX_DEPTH - 1; i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
        }
    }

    TEST_F(perft, perft_pos3) {
        searcher.setBoardFromFen(pos3);
        searcher.setMaxDepth(3);
        searcher.minimax(0, true, true);

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 14},
            {2, 191},
            {3, 2812},
            {4, 43238},
            {5, 674624}
        };

        for (int i = 1; i <= MAX_DEPTH; i++) {
            ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
        }
    }
}

