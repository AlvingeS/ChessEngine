#include "engine/search/searcher.h"
#include "engine/search/mvv_lva_table.h"

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

        // Find the highest capture value capture and swap it in
        if (i > 0) {
            int mvv_lva_max_idx = i;
            int mvv_lva_max_score = INT32_MIN;
            for (int j = i; j < move_lists_[ply].get_move_idx(); j++) {
                auto next_move_candidate = move_lists_[ply].get_move_at(j);
                if (next_move_candidate.is_any_capture()) {
                    PieceType attacker = pos_.bbs.get_piece_type_at(next_move_candidate.from());
                    PieceType victim   = pos_.bbs.get_piece_type_at(next_move_candidate.to());

                    if (MVV_LVA_TABLE[victim % 6][attacker % 6] > mvv_lva_max_score) {
                        mvv_lva_max_idx = j;
                        mvv_lva_max_score = MVV_LVA_TABLE[victim % 6][attacker % 6];
                    }
                }
            }

            move_lists_[ply].swap(i, mvv_lva_max_idx);
            move = move_lists_[ply].get_move_at(i);
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