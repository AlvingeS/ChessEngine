#include "engine/perft/perft.h"

#include "io/board_printer.h"
#include "model/constants.h"
#include "logic/masks.h"

#include <cstdlib>
#include <iostream>

namespace engine {

Perft::Perft(int max_depth) 
    : pos_(model::Position())
    , z_hasher_(pos_)
    , move_maker_(logic::MoveMaker(pos_, z_hasher_))
    , move_retractor_(logic::MoveRetractor(pos_, z_hasher_))
    , move_generator_(logic::MoveGen(pos_, move_maker_, move_retractor_))
    , eval_(logic::Eval(pos_))
    , max_depth_(max_depth)
{
    num_move_gen_calls_ = 0;
    total_nodes_ = 0;
    
    node_count_per_first_move_.resize(constants::MAX_LEGAL_MOVES);
    first_moves_.resize(constants::MAX_LEGAL_MOVES);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        node_count_per_first_move_[i] = 0;
        first_moves_[i] = model::Move();
    }

    undo_stack_.resize(max_depth_);
    move_lists_.resize(max_depth_);
    no_captures_or_pawn_moves_counts_.resize(max_depth_);

    node_count_.resize(20);
    capture_count_.resize(20);
    ep_capture_count_.resize(20);
    casle_count_.resize(20);
    promo_count_.resize(20);
    check_count_.resize(20);
    checkmate_count_.resize(20);

    for (int i = 0; i < 20; i++) {
        node_count_[i] = 0;
        capture_count_[i] = 0;
        ep_capture_count_[i] = 0;
        casle_count_[i] = 0;
        promo_count_[i] = 0;
        check_count_[i] = 0;
        checkmate_count_[i] = 0;
    }
}

long long Perft::sum_nodes_to_depth(int depth) const {
    long long sum = 0;

    for (long long i = 1; i <= depth; i++) {
        sum += node_count_[i];
    }

    return sum;
}

void Perft::debug_print(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(pos_.bbs);
        boardPrinter.print();
    }
}

void Perft::reset_stats() {
    num_move_gen_calls_ = 0;
    total_nodes_ = 0;
    
    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        node_count_per_first_move_[i] = 0;
        first_moves_[i] = model::Move();
    }

    for (int i = 0; i < 20; i++) {
        node_count_[i] = 0;
        capture_count_[i] = 0;
        ep_capture_count_[i] = 0;
        casle_count_[i] = 0;
        promo_count_[i] = 0;
        check_count_[i] = 0;
        checkmate_count_[i] = 0;
    }
}

void Perft::reset_stacks() {
    for (int i = 0; i < move_lists_.size(); i++) {
        move_lists_[i] = model::Movelist();
    }

    for (int i = 0; i < undo_stack_.size(); i++) {
        undo_stack_[i] = logic::UndoInfo();
    }

    for (int i = 0; i < no_captures_or_pawn_moves_counts_.size(); i++) {
        no_captures_or_pawn_moves_counts_[i] = 0;
    }
}

std::unordered_map<model::Move, uint64_t> Perft::get_node_count_per_first_move_map() 
{
    std::unordered_map<model::Move, uint64_t> node_count_per_first_move_map{};

    for (size_t i = 0; i < node_count_per_first_move_.size(); i++) {
        model::Move move = first_moves_[i];
        
        if (move.value() != 0)
            node_count_per_first_move_map[move] = node_count_per_first_move_[i];
    }

    return node_count_per_first_move_map;
}

void Perft::minimax(
    int current_depth, 
    int first_move_idx, 
    bool do_record_perft_stats, 
    const model::Move& last_move, 
    bool verbose)
{        
    if (current_depth == max_depth_) {
        return;
    }

    move_generator_.gen_moves(move_lists_[current_depth]);

    num_move_gen_calls_++;
    
    size_t num_illegal_moves = 0;

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move current_move = move_lists_[current_depth].get_move_at(i);

        // End of moves
        if (current_move.value() == 0) {
            break;
        }

        //  Make move
        undo_stack_[current_depth] = move_maker_.make_move(current_move);

        // Check if move is legal, unmake otherwise
        if (move_generator_.in_check(!pos_.is_w)) {
            num_illegal_moves++;
            move_retractor_.unmake_move(current_move, undo_stack_[current_depth]);

            if (num_illegal_moves == i + 1 && move_lists_[current_depth].get_move_at(i + 1).value() == 0) {
                bool was_in_check = move_generator_.in_check();

            if (was_in_check) {
                if (current_depth > 0) {
                    checkmate_count_[current_depth - 1]++;
                } else {
                    checkmate_count_[0]++;
                }
            }

                return;
            }

            continue;
        }

        if (do_record_perft_stats) {
            bool ret_flag;
            
            record_perft_stats(
                current_depth, 
                first_move_idx, 
                i, 
                current_move, 
                ret_flag
            );
            
            if (ret_flag)
                return;
        }

        minimax(
            current_depth + 1, 
            first_move_idx, 
            do_record_perft_stats, 
            current_move, 
            verbose
        );

        move_retractor_.unmake_move(current_move, undo_stack_[current_depth]);
    }

    return;
}

void Perft::record_perft_stats(
    int current_depth, 
    int &first_move_idx, 
    size_t i, 
    const model::Move& current_move, 
    bool &ret_flag) 
{
    ret_flag = true;
    if (move_generator_.in_check()) {
        check_count_[current_depth]++;
    }

    if (current_depth == 0) {
        first_move_idx = i;
        first_moves_[i] = current_move;
    }

    else if (current_depth == max_depth_ - 1) {
        node_count_per_first_move_[first_move_idx]++;
    }

    node_count_[current_depth]++;

    if (current_move.is_any_capture()) {
        capture_count_[current_depth]++;
    }

    if (current_move.is_any_promo()) {
        promo_count_[current_depth]++;
    }

    if (current_move.is_any_castle()) {
        casle_count_[current_depth]++;
    }

    if (current_move.is_ep_capture()) {
        ep_capture_count_[current_depth]++;
    }

    ret_flag = false;
}

} // namespace engine