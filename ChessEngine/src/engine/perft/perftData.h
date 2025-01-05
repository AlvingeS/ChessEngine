#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/move/Move.h"

namespace engine {

class perftData {

public:
    perftData(int maxDepth);

    long sumNodesToDepth(int depth) const;
    void resetNodeCounts();

    size_t getNumFirstMoves() const { return _firstMoves.size(); }
    int getNodeCountPerFirstMoveAt(int firstMoveIndex) const { return _nodeCountPerFirstMove[firstMoveIndex]; }

    inline int getNodeCountAt(int depth) const { return _nodeCount[depth]; }
    inline int getCaptureCountAt(int depth) const { return _captureCount[depth]; }
    inline int getEpCaptureCountAt(int depth) const { return _epCaptureCount[depth]; }
    inline int getCastlingCountAt(int depth) const { return _castlingCount[depth]; }
    inline int getPromotionCountAt(int depth) const { return _promotionCount[depth]; }
    inline int getCheckCountAt(int depth) const { return _checkCount[depth]; }
    inline int getCheckmateCountAt(int depth) const { return _checkmateCount[depth]; }

    inline void increaseNodeCountAt(int depth) { _nodeCount[depth]++; }
    inline void increaseCaptureCountAt(int depth) { _captureCount[depth]++; }
    inline void increaseEpCaptureCountAt(int depth) { _epCaptureCount[depth]++; }
    inline void increaseCastlingCountAt(int depth) { _castlingCount[depth]++; }
    inline void increasePromotionCountAt(int depth) { _promotionCount[depth]++; }
    inline void increaseCheckCountAt(int depth) { _checkCount[depth]++; }
    inline void increaseCheckmateCountAt(int depth) { _checkmateCount[depth]++; }

    model::Move getFirstMoveAt(int firstMoveIndex) const { return _firstMoves[firstMoveIndex]; }
    void setFirstMoveAt(int firstMoveIndex, model::Move currentMove) 
    {
        _firstMoves[firstMoveIndex] = currentMove;
    }

    void increaseNodeCountPerFirstMoveAt(int firstMoveIndex) 
    {
        _nodeCountPerFirstMove[firstMoveIndex]++;
    }

private:
    int _maxDepth;

    std::vector<long> _nodeCountPerFirstMove;
    std::vector<model::Move> _firstMoves;
    std::vector<long> _nodeCount;
    std::vector<long> _captureCount;
    std::vector<long> _epCaptureCount;
    std::vector<long> _castlingCount;
    std::vector<long> _promotionCount;
    std::vector<long> _checkCount;
    std::vector<long> _checkmateCount;
};

} // namespace engine