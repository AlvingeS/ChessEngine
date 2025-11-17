#pragma once

#include "model/move/Move.h"

namespace engine {

class perftData {

public:
    perftData(int maxDepth);

    long sumNodesToDepth(int depth) const;
    void resetNodeCounts();

    size_t getNumFirstMoves() const { return _firstMoves.size(); }
    int getNodeCountPerFirstMoveAt(int firstMoveIndex) const { return _nodeCountPerFirstMove[firstMoveIndex]; }

    int getNodeCountAt(int depth) const;
    int getCaptureCountAt(int depth) const;
    int getEpCaptureCountAt(int depth) const;
    int getCastlingCountAt(int depth) const;
    int getPromotionCountAt(int depth) const;
    int getCheckCountAt(int depth) const;
    int getCheckmateCountAt(int depth) const;

    void increaseNodeCountAt(int depth);
    void increaseCaptureCountAt(int depth);
    void increaseEpCaptureCountAt(int depth);
    void increaseCastlingCountAt(int depth);
    void increasePromotionCountAt(int depth);
    void increaseCheckCountAt(int depth);
    void increaseCheckmateCountAt(int depth);

    model::Move getFirstMoveAt(int firstMoveIndex) const { return _firstMoves[firstMoveIndex]; }
    void setFirstMoveAt(int firstMoveIndex, model::Move currentMove);
    void increaseNodeCountPerFirstMoveAt(int firstMoveIndex);

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