#include "perftBase.h"

namespace search {
    class perftPosFive : public perftBase {
        protected:
            void SetUp() override {
                perftBase::SetUp();
            }

        // Nodes
        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 44},
            {2, 1486},
            {3, 62379},
            {4, 2103487},
            {5, 89941194}
        };

    };

    TEST_F(perftPosFive, perft_pos5) {
        if (enablePos5Test) {
            int depth = longRuns ? posFiveMaxDepth + 1 : posFiveMaxDepth;
            std::unordered_map<std::string, int> stockfishResults = getStockFishPerftResults(posFive, depth);

            searcher.setMaxDepth(depth);
            searcher.setBoardFromFen(posFive);
            searcher.getBoard().setKingMoved(false, true);
            bool whiteToStart = true;

            searcher.minimax(0, whiteToStart, 0);

            std::unordered_map<std::string, int> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
            compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}