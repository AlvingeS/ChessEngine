#include "perftBase_test.h"

namespace ponder {
namespace perft {

class perftPosTwo : public perftBase 
{
protected:
    void SetUp() override {
        perftBase::SetUp();
    }

    // Nodes, captures, epCaptures, castling, promotion, check, checkmate
    std::unordered_map<int, std::vector<long>> expectedResults {
        {0, {1,         0,          0,          0,          0,          0,          0}},
        {1, {48,        8,          0,          2,          0,          0,          0}},
        {2, {2039,      351,        1,          91,         0,          3,          0}},
        {3, {97862,     17102,      45,         3162,       0,          993,        1}},
        {4, {4085603,   757163,     1929,       128013,     15172,      25523,      43}},
        {5, {193690690, 35043416,   73365,      4993637,    8392,       3309887,    30171}},
        {6, {0,         0,          0,          0,          0,          0,          0}}
    };
};

TEST_F(perftPosTwo, perft_pos2) 
{
    if (enablePos2Test) {
        engine.resetToFEN(posTwo);

        // Make dubug move
        // game::move::Move move = moveFromStrAndFlag("a2a3", 0);
        // searcher.makeMove(move, true);
        // move = moveFromStrAndFlag("c7c6", 0);
        // searcher.makeMove(game::move::Move(0, 0, game::move::Move::QUEEN_CASTLE_FLAG), false);

        int nDebugMoves = 0;

        int depth = longRuns ? posTwoMaxDepth + 1 : posTwoMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = engine.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " KQkq -";
        }

        std::unordered_map<std::string, long> stockfishResults = getStockFishPerftResults(nDebugMoves > 0 ? debugFen : posTwo, depth);

        engine.setMaxSearchDepth(depth);
        engine.runPerft(0, whiteToStart, 0, true);

        std::unordered_map<std::string, long> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        if (nDebugMoves == 0) {
            for (long i = 1; i <= engine.getMaxSearchDepth(); i++) {
                ASSERT_EQ(engine.getNodeCount()[i], expectedResults[i][0]);
                ASSERT_EQ(engine.getCaptureCount()[i], expectedResults[i][1]);
                ASSERT_EQ(engine.getEpCaptureCount()[i], expectedResults[i][2]);
                ASSERT_EQ(engine.getCastlingCount()[i], expectedResults[i][3]);
                ASSERT_EQ(engine.getPromotionCount()[i], expectedResults[i][4]);
                ASSERT_EQ(engine.getCheckCount()[i], expectedResults[i][5]);
                
                if (i < engine.getMaxSearchDepth()) {
                    ASSERT_EQ(engine.getCheckmateCount()[i], expectedResults[i][6]);
                }       
            }
        }
    }
}

} // namespace perft
} // namespace ponder