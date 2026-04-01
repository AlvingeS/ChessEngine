#include "engine/search/game_history.h"

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

bool GameHistory::is_move_irreversible(const model::Move& move, bool castle_rights_changed)
{
        return move.is_any_capture() 
            || move.flag() == model::Move::SINGLE_PAWN_PUSH_FLAG 
            || move.flag() == model::Move::DOUBLE_PAWN_PUSH_FLAG
            || move.is_any_promo()
            || castle_rights_changed;
}

} // namespace engine