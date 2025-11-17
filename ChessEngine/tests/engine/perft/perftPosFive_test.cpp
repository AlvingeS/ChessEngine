#include "perftBase_test.h"

#include "io/StockfishPerftRetriever.h"
namespace engine {

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
        movePicker.setBoardFromFen(posFive);

        // Make dubug move
        // model::Move move = moveFromStrAndFlag("a2a3", 0);
        // movePicker.makeMove(move, true);
        int nDebugMoves = 0;

        int depth = longRuns ? posFiveMaxDepth + 1 : posFiveMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = movePicker.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " KQ - 1 8";
        }

        std::unordered_map<std::string, uint64_t> stockfishResults = io::stockfish::getPerftResults(nDebugMoves > 0 ? debugFen : posFive, depth);

        movePicker.setMaxDepth(depth);
        movePicker.minimax(0, whiteToStart, 0);

        std::unordered_map<std::string, uint64_t> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        if (nDebugMoves == 0) {
            for (uint64_t i = 1; i <= movePicker.getMaxDepth(); i++) {
                ASSERT_EQ(movePicker._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}

} // namespace engine