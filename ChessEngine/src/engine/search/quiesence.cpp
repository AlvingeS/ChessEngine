#include "engine/search/searcher.h"

namespace engine {

eval_t Searcher::quiescence(eval_t alpha, eval_t beta, int ply, const TimeManager& tm) {
    node_count_++;
    if (node_count_ % NODE_CHECK_INTERVAL == 0) {
        if (tm.time_is_up()) {
            stop_ = true;
            return 0;
        }
    }

    if (stop_) return 0;

    bool in_check = move_generator_.in_check();

    if (!in_check) {
        eval_t stand_pat = eval_.evaluate();
        if (stand_pat >= beta) return beta;
        alpha = std::max(alpha, stand_pat);
    }

    bool captures_only = !in_check;
    auto legality_info = move_generator_.gen_moves(move_lists_[ply], captures_only);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move move = move_lists_[ply].get_move_at(i);

        if (move.value() == 0) {
            // If i == 0 then there are no legal moves
            if (i == 0) {
                if (legality_info.in_check()) {
                    return -BIG_NUMBER + ply;
                }
            }
            break;
        }

        undo_stack_[ply] = move_maker_.make_move(move);

        // If move is ep, make the move, check if in check, if so - undo
        if (move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(move, undo_stack_[ply]);   
                continue;
            }
        }

        eval_t score = -quiescence(-beta, -alpha, ply + 1, tm);
        
        move_retractor_.unmake_move(move, undo_stack_[ply]);
        
        if (stop_) break;

        alpha = std::max(score, alpha);

        if (alpha >= beta) {
            // Prune!
            break;
        }
    }

    return alpha;
}

} // namespace engine