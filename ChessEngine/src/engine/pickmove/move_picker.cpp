#include "engine/pickmove/move_picker.h"


#include "io/board_printer.h"
#include "model/constants.h"
#include "logic/masks.h"

#include <cstdlib>
#include <iostream>

namespace engine {

MovePicker::MovePicker() 
    : pos_(model::Position())
    , z_hasher_(pos_)
    , move_maker_(logic::MoveMaker(pos_, z_hasher_))
    , move_retractor_(logic::MoveRetractor(pos_, z_hasher_))
    , move_generator_(logic::MoveGen(pos_))
    , eval_(logic::Eval(pos_))
{
    move_lists_.resize(MAX_SAFE_DEPTH + 1);
    undo_stack_.resize(MAX_SAFE_DEPTH + 1);
}

model::Move MovePicker::pick_move(TimeManager& tm) {
    model::Move best_move_completed;
    int max_depth = tm.get_depth().value_or(MAX_SAFE_DEPTH);

    stop_ = false;
    node_count_ = 0;
    tm.start();

    for (int depth = 1; depth <= max_depth; depth++) {
        model::Move best_move_this_iteration = model::Move();
        int best_score_this_iteration = -BIG_NUMBER;
        
        auto ml = model::Movelist();
        auto undo_info = logic::UndoInfo();
        move_generator_.gen_moves(ml);

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
    
            int score = -negamax(depth - 1, -BIG_NUMBER, BIG_NUMBER, tm);
            move_retractor_.unmake_move(move, undo_info);
    
            if (stop_) break;

            if (score > best_score_this_iteration) {
                best_score_this_iteration = score;
                best_move_this_iteration = move;
            }
        }

        if (stop_) break;

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
                  << "\n";

        if (tm.time_is_up()) break;
    }

    return best_move_completed;
}

int MovePicker::negamax(int depth, int alpha, int beta, const TimeManager& tm) {
    node_count_++;
    if (node_count_ % NODE_CHECK_INTERVAL == 0) {
        if (tm.time_is_up()) {
            stop_ = true;
            return 0;
        }
    }

    if (stop_) return 0;

    if (depth == 0) {
        return eval_.evaluate();
    }
    
    move_generator_.gen_moves(move_lists_[depth - 1]);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move move = move_lists_[depth - 1].get_move_at(i);

        if (move.value() == 0) {
            break;
        }

        undo_stack_[depth - 1] = move_maker_.make_move(move);

        // If move is ep, make the move, check if in check, if so - undo
        if (move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(move, undo_stack_[depth - 1]);   
                continue;
            }
        }

        int score = -negamax(depth - 1, -beta, -alpha, tm);

        move_retractor_.unmake_move(move, undo_stack_[depth - 1]);

        alpha = std::max(score, alpha);

        if (alpha >= beta) {
            // Prune!
            break;
        }
    }  

    return alpha;
}

void MovePicker::reset_stacks() {
    for (long long i = 0; i < static_cast<long long>(move_lists_.size()); i++) {
        move_lists_[i] = model::Movelist();
    }

    for (long long i = 0; i < static_cast<long long>(undo_stack_.size()); i++) {
        undo_stack_[i] = logic::UndoInfo();
    }
}

} // namespace engine