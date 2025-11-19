#include "engine/perft/perft_data.h"

#include "model/constants.h"

namespace engine {

perftData::perftData(int maxDepth)
    : max_depth_(maxDepth)
{
    node_count_per_first_move_.resize(constants::MAX_LEGAL_MOVES);
    first_moves_.resize(constants::MAX_LEGAL_MOVES);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        node_count_per_first_move_[i] = 0;
        first_moves_[i] = model::Move();
    }

    node_count_.resize(max_depth_ + 1);
    capture_count_.resize(max_depth_ + 1);
    ep_capture_count_.resize(max_depth_ + 1);
    casle_count_.resize(max_depth_ + 1);
    promo_count_.resize(max_depth_ + 1);
    check_count_.resize(max_depth_ + 1);
    checkmate_count_.resize(max_depth_ + 1);

    for (int i = 0; i < max_depth_ + 1; i++) {
        node_count_[i] = 0;
        capture_count_[i] = 0;
        ep_capture_count_[i] = 0;
        casle_count_[i] = 0;
        promo_count_[i] = 0;
        check_count_[i] = 0;
        checkmate_count_[i] = 0;
    }
}

long perftData::sum_nodes_to_depth(int depth) const 
{
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += node_count_[i];
    }

    return sum;
}

void perftData::resetNodeCounts() 
{
    for (int i = 0; i < max_depth_ + 1; i++) {
        node_count_[i] = 0;
        capture_count_[i] = 0;
        ep_capture_count_[i] = 0;
        casle_count_[i] = 0;
        promo_count_[i] = 0;
        check_count_[i] = 0;
        checkmate_count_[i] = 0;
    }
}

} // namespace engine