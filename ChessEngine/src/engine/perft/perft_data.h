#pragma once

#include "model/move/move.h"

namespace engine {

class perftData {

public:
    perftData(int maxDepth);

    long sum_nodes_to_depth(int depth) const;
    void resetNodeCounts();

    size_t getNumFirstMoves() const { return first_moves_.size(); }
    int getNodeCountPerFirstMoveAt(int firstMoveIndex) const { return node_count_per_first_move_[firstMoveIndex]; }

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

    model::Move getFirstMoveAt(int firstMoveIndex) const { return first_moves_[firstMoveIndex]; }
    void setFirstMoveAt(int firstMoveIndex, model::Move currentMove);
    void increaseNodeCountPerFirstMoveAt(int firstMoveIndex);

private:
    int max_depth_;

    std::vector<long> node_count_per_first_move_;
    std::vector<model::Move> first_moves_;
    std::vector<long> node_count_;
    std::vector<long> capture_count_;
    std::vector<long> ep_capture_count_;
    std::vector<long> casle_count_;
    std::vector<long> promo_count_;
    std::vector<long> check_count_;
    std::vector<long> checkmate_count_;
};

} // namespace engine