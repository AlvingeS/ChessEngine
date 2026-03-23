#include "engine/perft/perft.h"

#include "model/constants.h"

#include "logic/movegen/check_detection.h"
#include "logic/masks.h"
#include "io/board_printer.h"


#include <cstdlib>
#include <iostream>

namespace engine {

Perft::Perft(int max_depth) 
    : pos_(model::Position())
    , z_hasher_(pos_)
    , move_maker_(logic::MoveMaker(pos_, z_hasher_))
    , move_retractor_(logic::MoveRetractor(pos_, z_hasher_))
    , move_generator_(logic::MoveGen(pos_))
    , eval_(logic::Eval(pos_))
    , max_depth_(max_depth)
{
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
    double_check_count_.resize(20);
    checkmate_count_.resize(20);

    for (int i = 0; i < 20; i++) {
        node_count_[i] = 0;
        capture_count_[i] = 0;
        ep_capture_count_[i] = 0;
        casle_count_[i] = 0;
        promo_count_[i] = 0;
        check_count_[i] = 0;
        double_check_count_[i] = 0;
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

void Perft::reset_stats() {
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
    for (long long i = 0; i < static_cast<long long>(move_lists_.size()); i++) {
        move_lists_[i] = model::Movelist();
    }

    for (long long i = 0; i < static_cast<long long>(undo_stack_.size()); i++) {
        undo_stack_[i] = logic::UndoInfo();
    }

    for (long long i = 0; i < static_cast<long long>(no_captures_or_pawn_moves_counts_.size()); i++) {
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
    bool verbose)
{        
    if (current_depth == max_depth_) {
        return;
    }
    
    if (verbose) {
        std::string w = pos_.is_w ? "white" : "black";
        std::cout << "Generating moves from the following position for " + w + "..." << std::endl;
        auto bp =io::BoardPrinter(pos_);
        bp.print();
        std::cout << "Making moves..." << std::endl;
    }
    logic::LegalityInfo legality_info = move_generator_.gen_moves(move_lists_[current_depth]);

    // The move that was made before the next call of minimax put us in check, at the previous depth
    if (current_depth - 1 >= 0 && legality_info.in_check()) {
        check_count_[current_depth - 1]++;
    }

    if (current_depth - 1 >= 0 && legality_info.in_double_check()) {
        double_check_count_[current_depth - 1]++;
    }

    if (current_depth - 1 >= 0 && legality_info.in_check() && move_lists_[current_depth].get_move_idx() == 0) {
        checkmate_count_[current_depth - 1]++;
    }

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move current_move = move_lists_[current_depth].get_move_at(i);

        // End of moves
        if (current_move.value() == 0) {
            break;
        } 

        //  Make move
        undo_stack_[current_depth] = move_maker_.make_move(current_move);
        if (verbose) {
            auto bp =io::BoardPrinter(pos_);
            bp.print(current_move);
         
            std::optional<bitmask> debug_occ_mask = std::nullopt;
            int debug_move_value = 4690;

            if (debug_occ_mask.has_value()) {
                if (pos_.bbs.get_occ() == debug_occ_mask.value() &&
                    current_move.value() == debug_move_value) 
                {
                    std::cout << "Debug state reached!" << std::endl;
                }
            } else if (current_move.value() == debug_move_value) {
                std::cout << "Debug state reached!" << std::endl;
            }
        }

        // If move is ep, make the move, check if in check, if so - undo
        if (current_move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(current_move, undo_stack_[current_depth]);   
                continue;
            }
        }

        record_perft_stats(
            current_depth, 
            first_move_idx, 
            i, 
            current_move
        );

        minimax(
            current_depth + 1, 
            first_move_idx, 
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
    const model::Move& current_move) 
{
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
}

} // namespace engine