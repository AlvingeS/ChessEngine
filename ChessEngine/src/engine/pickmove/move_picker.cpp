#include "engine/pickmove/move_picker.h"

#include "io/board_printer.h"
#include "model/constants.h"
#include "logic/masks.h"

#include <cstdlib>
#include <iostream>
namespace engine {

MovePicker::MovePicker(int max_depth) 
    : pos_(model::Position())
    , z_hasher_(pos_)
    , move_maker_(logic::MoveMaker(pos_, z_hasher_))
    , move_retractor_(logic::MoveRetractor(pos_, z_hasher_))
    , move_generator_(logic::MoveGen(pos_))
    , eval_(logic::Eval(pos_))
    , max_depth_(max_depth)
{
    move_lists_.resize(max_depth_);
    undo_stack_.resize(max_depth_);
}

model::Move MovePicker::pick_move(int depth) {
    model::Move best_move;
    int best_score = -big_number_;

    auto ml = model::Movelist();
    auto undo_info = logic::UndoInfo();
    move_generator_.gen_moves(ml);

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

        int score = -negamax(depth, -big_number_, big_number_);
        move_retractor_.unmake_move(move, undo_info);

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    return best_move;
}

int MovePicker::negamax(int depth, int alpha, int beta) {
    if (depth == 0) {
        return eval_.evaluate();
    }
    
    move_generator_.gen_moves(move_lists_[depth]);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move move = move_lists_[depth].get_move_at(i);

        if (move.value() == 0) {
            break;
        }

        undo_stack_[depth] = move_maker_.make_move(move);

        // If move is ep, make the move, check if in check, if so - undo
        if (move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(move, undo_stack_[depth]);   
                continue;
            }
        }

        int score = -negamax(depth - 1, -beta, -alpha);

        move_retractor_.unmake_move(move, undo_stack_[depth]);

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