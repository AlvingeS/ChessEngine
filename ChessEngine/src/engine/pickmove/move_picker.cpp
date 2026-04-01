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
    , tt_(engine::TT())
    , game_hist_(GameHistory())
{
    move_lists_.resize(MAX_SAFE_DEPTH + 1);
    undo_stack_.resize(MAX_SAFE_DEPTH + 1);
}

model::Move MovePicker::pick_move(TimeManager& tm) {
    model::Move best_move_completed;
    int max_depth = tm.get_depth().value_or(MAX_SAFE_DEPTH);

    stop_ = false;
    node_count_ = 0;
    tt_hits_ = 0;
    tm.start();

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
                  << " string tt_hits " << tt_hits_
                  << "\n";

        if (tm.time_is_up()) break;
    }

    return best_move_completed;
}

eval_t MovePicker::negamax(int depth, eval_t alpha, eval_t beta, int ply, const TimeManager& tm) {
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
        return eval_.evaluate();
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
        if (is_move_irreversible(move, undo_stack_[ply].c_rights)) {
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

bool MovePicker::is_move_irreversible(const model::Move& move, const castle_rights previous_c_rights) const
{
        return move.is_any_capture() 
            || move.flag() == model::Move::SINGLE_PAWN_PUSH_FLAG 
            || move.flag() == model::Move::DOUBLE_PAWN_PUSH_FLAG
            || move.is_any_promo()
            || pos_.c_rights != previous_c_rights;
}

void MovePicker::make_move(const model::Move& move) {
    castle_rights prev_c_rights = pos_.c_rights;
    move_maker_.make_move(move);
    if (is_move_irreversible(move, prev_c_rights)) {
        game_hist_.push_irreversible(z_hasher_.value());
    } else {
        game_hist_.push(z_hasher_.value());
    }
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