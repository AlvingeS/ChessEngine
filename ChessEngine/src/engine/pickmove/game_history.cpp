#include "engine/pickmove/game_history.h"

namespace engine {

int GameHistory::count(uint64_t hash) 
{
    if (idx_ - last_irreversible_move_idx_ < 4)
        return 0;

    int count = 0;
    for (int i = idx_ - 3; i > last_irreversible_move_idx_; i -= 2) {
        if (arr_[i] == hash)
            count++;
    }

    return count;
}

void GameHistory::clear()
{
    idx_ = 0; 
    last_irreversible_move_idx_ = 0; 
}

} // namespace engine