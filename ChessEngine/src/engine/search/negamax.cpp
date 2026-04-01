#include "engine/search/searcher.h"

namespace engine {

eval_t Searcher::negamax(int depth, eval_t alpha, eval_t beta, int ply, const TimeManager& tm) {
    node_count_++;
    if (node_count_ % NODE_CHECK_INTERVAL == 0) {
        if (tm.time_is_up()) {
            stop_ = true;
            return 0;
        }
    }

    if (stop_) return 0;

    auto tt_entry = tt_.lookup(z_hasher_.value()); 
    bool tt_miss = tt_entry == nullptr;
    if (!tt_miss && tt_entry->depth >= depth) {
        switch (tt_entry->flag) {
            case TTFlag::EXACT:
                tt_hits_++;
                return tt_entry->score;
            case TTFlag::LOWER_BOUND:
                alpha = std::max(alpha, tt_entry->score);
                break;
            case TTFlag::UPPER_BOUND:
                beta = std::min(beta, tt_entry->score);
                break;
            case TTFlag::NONE:
                break;
        }
        if (alpha >= beta) {
            tt_hits_++;
            return alpha;
        }
    }

    if (depth == 0) {
        return quiescence(alpha, beta, ply, tm);
    }

    eval_t original_alpha = alpha;
    
    auto legality_info = move_generator_.gen_moves(move_lists_[ply], false);

    if (!tt_miss) {
        int swap_idx = 0;
        for (int i = 0; i < move_lists_[ply].get_move_idx(); i++) {
            if (move_lists_[ply].get_move_at(i).value() == tt_entry->best_move_val) {
                swap_idx = i;
                break;
            }
        }

        move_lists_[ply].swap(0, swap_idx);
    }

    model::Move best_move = model::Move();
    eval_t best_score = -BIG_NUMBER;
    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move move = move_lists_[ply].get_move_at(i);

        if (move.value() == 0) {
            // If i == 0 then there are no legal moves
            if (i == 0) {
                if (legality_info.in_check()) {
                    tt_.store(z_hasher_.value(), -BIG_NUMBER + ply, depth, TTFlag::EXACT, 0);
                    return -BIG_NUMBER + ply;
                } else {
                    tt_.store(z_hasher_.value(), 0, depth, TTFlag::EXACT, 0);
                    return 0;
                }
            }
            break;
        }

        int previous_last_irreversible_move_idx = game_hist_.get_last_irreversible_move_idx();
        undo_stack_[ply] = move_maker_.make_move(move);

        // If move is ep, make the move, check if in check, if so - undo
        if (move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(move, undo_stack_[ply]);   
                continue;
            }
        }

        uint64_t z_hash = z_hasher_.value();
        if (GameHistory::is_move_irreversible(move, pos_.c_rights != undo_stack_[ply].c_rights)) {
            game_hist_.push_irreversible(z_hash);
        } else {
            game_hist_.push(z_hash);
        }

        // We have reached this position 2 times before, and just moved into it again
        // this means that we end the game in draw by three-fold repetition
        int repetitions = game_hist_.count(z_hash);
        if (repetitions == 2 || (repetitions == 1 && ply > 2)) {
            move_retractor_.unmake_move(move, undo_stack_[ply]);
            game_hist_.pop(previous_last_irreversible_move_idx);
            return 0;
        }

        eval_t score = -negamax(depth - 1, -beta, -alpha, ply + 1, tm);
        
        move_retractor_.unmake_move(move, undo_stack_[ply]);
        game_hist_.pop(previous_last_irreversible_move_idx);
        if (stop_) break;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }

        alpha = std::max(score, alpha);

        if (alpha >= beta) {
            // Prune!
            break;
        }
    }

    if (stop_) return alpha;

    TTFlag flag = TTFlag::EXACT;
    if (alpha >= beta) {
        flag = TTFlag::LOWER_BOUND;
    } else if (alpha == original_alpha) {
        flag = TTFlag::UPPER_BOUND;
    }

    tt_.store(z_hasher_.value(), alpha, depth, flag, best_move.value());

    return alpha;
}

} // namespace engine