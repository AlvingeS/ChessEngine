#include "perftBase_test.h"

namespace ponder {
namespace perft {

class perftPosFive : public perftBase 
{
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

TEST_F(perftPosFive, perft_pos5) 
{
    if (enablePos5Test) {
        searcher.setBoardFromFen(posFive);

        // Make dubug move
        // game::move::Move move = moveFromStrAndFlag("a2a3", 0);
        // searcher.makeMove(move, true);
        int nDebugMoves = 0;

        int depth = longRuns ? posFiveMaxDepth + 1 : posFiveMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = searcher.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " KQ - 1 8";
        }

        std::unordered_map<std::string, long> stockfishResults = getStockFishPerftResults(nDebugMoves > 0 ? debugFen : posFive, depth);

        searcher.setMaxDepth(depth);
        searcher.minimax(0, whiteToStart, 0);

        std::unordered_map<std::string, long> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        if (nDebugMoves == 0) {
            for (long i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}

} // namespace perft
} // namespace ponder