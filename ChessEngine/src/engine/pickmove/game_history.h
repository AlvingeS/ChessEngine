#pragma once

#include <array>
#include <cstdint>

namespace engine {



class GameHistory
{

public:
    inline void push(uint64_t hash) { arr_[idx_++] = hash; }
    inline void push_irreversible(uint64_t hash) { last_irreversible_move_idx_ = idx_; push(hash); }
    inline void pop(int previous_irreversible_move_idx) { idx_--; last_irreversible_move_idx_ = previous_irreversible_move_idx; }
    void clear();
    inline  int get_last_irreversible_move_idx() const { return last_irreversible_move_idx_; }

    int count(uint64_t hash);
private:
    static constexpr int MAX_GAME_LENGTH{4096};
   
    int idx_{0};
    int last_irreversible_move_idx_{0};
    std::array<uint64_t, MAX_GAME_LENGTH> arr_;
};

} // namespace engine