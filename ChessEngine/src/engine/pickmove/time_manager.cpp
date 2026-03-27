#include "engine/pickmove/time_manager.h"

#include <stdexcept>

namespace engine {

TimeManager::TimeManager(const uci::GoParams& go_params, bool is_w)
{
    if (go_params.movetime.has_value()) {
        allocated_ms_ = go_params.movetime.value();
        return;
    }

    if (go_params.depth.has_value()) {
        allocated_ms_ = LARGE_NUMBER;
        depth_ = go_params.depth.value();
        return;
    }

    if (go_params.infinite.has_value()) {
        allocated_ms_ = LARGE_NUMBER;
        return;
    }

    if ((is_w && !go_params.wtime.has_value()) ||
       (!is_w && !go_params.btime.has_value())) {
        throw std::runtime_error("Did not receive remaining time.");
    }

    if (is_w) {
        allocated_ms_ = go_params.wtime.value() / 20 + go_params.winc.value_or(0) * 0.8;
    } else {
        allocated_ms_  = go_params.btime.value() / 20 + go_params.binc.value_or(0) * 0.8;
    }
}

void TimeManager::start()
{
    start_time_ = std::chrono::steady_clock::now();
    deadline_ = start_time_ + std::chrono::milliseconds(allocated_ms_);   
}


} // namespace engine