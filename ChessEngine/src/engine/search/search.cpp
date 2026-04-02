#include "engine/search/searcher.h"

#include <iostream>

namespace engine {

model::Move Searcher::search(TimeManager& tm) {
    model::Move best_move_completed;
    int max_depth = tm.get_depth().value_or(MAX_SAFE_DEPTH);

    stop_ = false;
    node_count_ = 0;
    tt_hits_ = 0;
    killer_move_swaps_ = 0;
    tm.start();
    killer_moves_ = {};

    for (int depth = 1; depth <= max_depth; depth++) {
        model::Move best_move_this_iteration = model::Move();
        int best_score_this_iteration = -BIG_NUMBER;
        
        auto ml = model::Movelist();
        auto undo_info = logic::UndoInfo();
        move_generator_.gen_moves(ml, false);

        // Put the best move found first for the next iteration
        if (depth > 1) {
            int swap_idx = 0;
            for (int i = 0; i < ml.get_move_idx(); i++) {
                if (ml.get_move_at(i) == best_move_completed) {
                    swap_idx = i;
                    break;
                }
            }

            ml.swap(0, swap_idx);
        }

        for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
            model::Move move = ml.get_move_at(i);
    
            if (move.value() == 0) {
                break;
            }
    
            undo_info = move_maker_.make_move(move);
    
            // If move is ep, make the move, check if in check, if so - undo
            if (move.is_ep_capture()) {
                if (move_generator_.in_check())  {
                    move_retractor_.unmake_move(move, undo_info);   
                    continue;
                }
            }
    
            int score = -negamax(depth - 1, -BIG_NUMBER, BIG_NUMBER, 1, tm);
            move_retractor_.unmake_move(move, undo_info);
    
            if (stop_) break;

            if (score > best_score_this_iteration) {
                best_score_this_iteration = score;
                best_move_this_iteration = move;
            }
        }

        if (stop_) {
            if (best_move_completed.value() == 0) {
                best_move_completed = best_move_this_iteration;
            }
            break;
        };

        // This code is only reached on completed runs
        best_move_completed = best_move_this_iteration;
        int uci_score = best_score_this_iteration / 10;
        if (!pos_.is_w) uci_score = -uci_score;

        std::cout << "info"
                  << " depth " << depth
                  << " score cp " << uci_score
                  << " nodes " << node_count_
                  << " nps " <<  1000 * (node_count_ / std::max(1, tm.get_elapsed_ms()))
                  << " time " << tm.get_elapsed_ms()
                  << " hashfull " << tt_.hashfull()
                  << " string tt hitrate " << (tt_hits_ * 100.0) / node_count_
                  << " string km swaps " << killer_move_swaps_
                  << "\n";

        if (tm.time_is_up() || stop_) break;
    }

    return best_move_completed;
}

} // namespace engine