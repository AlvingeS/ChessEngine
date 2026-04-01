#pragma once

#include "engine/uci/go_params.h"

#include <chrono>

namespace engine {


class TimeManager
{
    
    public:
    TimeManager(const uci::GoParams& go_params, bool is_w);
    
    void start();
    inline bool time_is_up() const { return std::chrono::steady_clock::now() > deadline_; }
    
    inline int get_elapsed_ms() const { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time_).count(); }
    inline int get_allocated_time_ms() const { return allocated_ms_; }
    inline std::optional<int> get_depth() const { return depth_; }

private:
    int allocated_ms_;
    std::optional<int> depth_{std::nullopt};
    static constexpr int    REMAINING_MOVES_ESTIMATE = 20;
    static constexpr double INC_SCALE = 0.8;
    static constexpr int    LARGE_NUMBER = 999999999;
    std::chrono::steady_clock::time_point deadline_;
    std::chrono::steady_clock::time_point start_time_;
};

} // namespace engine