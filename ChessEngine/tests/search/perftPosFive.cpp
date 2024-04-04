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
        searcher.setMaxDepth(longRuns ? 5 : 4);
        searcher.setBoardFromFen(posFive);
        searcher.getBoard().setHasCastled(false, true);

        if (enablePos5Test) {
            searcher.minimax(0, true, 0);
        }

        if (enablePos5Test) {
            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}