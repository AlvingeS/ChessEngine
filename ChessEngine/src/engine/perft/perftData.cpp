#include "ChessEngine/src/engine/perft/perftData.h"

#include "ChessEngine/src/logic/movegen/MoveGenerator.h"

namespace engine {
namespace perft {

perftData::perftData(int maxDepth)
    : _maxDepth(maxDepth)
{
    _nodeCountPerFirstMove.resize(logic::movegen::MoveGenerator::MAX_LEGAL_MOVES);
    _firstMoves.resize(logic::movegen::MoveGenerator::MAX_LEGAL_MOVES);

    for (int i = 0; i < logic::movegen::MoveGenerator::MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = representation::move::Move();
    }

    _nodeCount.resize(_maxDepth + 1);
    _captureCount.resize(_maxDepth + 1);
    _epCaptureCount.resize(_maxDepth + 1);
    _castlingCount.resize(_maxDepth + 1);
    _promotionCount.resize(_maxDepth + 1);
    _checkCount.resize(_maxDepth + 1);
    _checkmateCount.resize(_maxDepth + 1);

    for (int i = 0; i < _maxDepth + 1; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

long perftData::sumNodesToDepth(int depth) const 
{
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void perftData::resetNodeCounts() 
{
    for (int i = 0; i < _maxDepth + 1; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

} // namespace perft
} // namespace engine