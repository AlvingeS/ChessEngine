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

long MovePicker::sum_nodes_to_depth(int depth) const {
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += node_count_[i];
    }

    return sum;
}

void MovePicker::debug_print(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(pos_.bbs);
        boardPrinter.print();
    }
}

// Helper function to check if there are any castling moves in the movelist
// bool hasTwoCastlingMove(MoveList& movelist) {
//     int count = 0;

//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].is_any_castle()) {
//             count++;
//         }
//     }

//     return count == 2;
// }

// bool noKingSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].get_flag() == 3) {
//             return false;
//         }
//     }

//     return true;
// }

// bool noQueenSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].get_flag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool MovePicker::too_many_pieces_on_board() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (pos_.piece_map.get_piece_type_at(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool MovePicker::check_condition(
    int current_depth,
    bool is_maximizer,
    int first_move_idx, 
    model::Move current_move, 
    model::Move last_move, 
    bool verbose, 
    size_t i) const
{
    static_cast<void>(current_depth);
    static_cast<void>(is_maximizer);
    static_cast<void>(first_move_idx);
    static_cast<void>(current_move);
    static_cast<void>(last_move);
    static_cast<void>(verbose);
    static_cast<void>(i);

    // return not board_.getKingMoved(false);
    // return too_many_pieces_on_board();
    // return first_move_idx == 19 && current_move.is_any_capture();
    // return current_move.get_from_sq() == 12 && current_move.get_to_sq() == 12;
    // return current_depth == 3 && first_move_idx == 0 && current_move.get_from_sq() == 34 && current_move.get_to_sq() == 27;
    // return current_move.is_any_capture();
    // return true;
    return false;
    // return diff_between_occupancy_masks();
    // return current_depth == 2 && first_move_idx == 0 && is_maximizer == true && current_move.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void MovePicker::minimax(
    int current_depth, 
    bool is_maximizer, 
    int first_move_idx, 
    bool do_record_perft_stats, 
    const model::Move& last_move, 
    bool verbose)
{        
    if (current_depth == max_depth_) {
        return;
    }

    move_generator_.gen_moves(
        is_maximizer, 
        move_lists_[current_depth]
    );

    num_move_gen_calls_++;
    
    size_t num_illegal_moves = 0;

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) { // If I just do auto move : blah blah wont that keep track of the moves instead of movelists?
        model::Move current_move = move_lists_[current_depth].get_move_at(i);

        // End of moves
        if (current_move.value() == 0) {
            break;
        }

        //  Make move
        undo_stack_[current_depth] = move_maker_.make_move(current_move, is_maximizer);

        // Check if move is legal, unmake otherwise
        if (move_generator_.in_check(is_maximizer)) {
            num_illegal_moves++;
            move_retractor_.unmake_move(current_move, is_maximizer, undo_stack_[current_depth]);

            if (num_illegal_moves == i + 1 && move_lists_[current_depth].get_move_at(i + 1).value() == 0) {
                bool was_in_check = move_generator_.in_check(is_maximizer);

                if (was_in_check) {
                    checkmate_count_[current_depth]++;
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights

        if (do_record_perft_stats) {
            bool ret_flag;
            
            record_perft_stats(
                is_maximizer, 
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
            !is_maximizer, 
            first_move_idx, 
            do_record_perft_stats, 
            current_move, 
            verbose
        );

        move_retractor_.unmake_move(current_move, is_maximizer, undo_stack_[current_depth]);
    
        // if (not current_move.is_any_capture() && (move_result.captured_piece_type != model::Piece::Type::W_PAWN && move_result.moved_piece_type != model::Piece::Type::B_PAWN)) {
        //     search_memory_.decrement_no_captures_or_pawn_moves_count_at_depth(current_depth + 1);
        // }
    }

    return;
}

void MovePicker::record_perft_stats(
    bool is_maximizer, 
    int current_depth, 
    int &first_move_idx, 
    size_t i, 
    const model::Move& current_move, 
    bool &ret_flag) 
{
    ret_flag = true;
    if (move_generator_.in_check(!is_maximizer))
    {
        check_count_[current_depth + 1]++;
    }

    if (current_depth == 0) {
        first_move_idx = i;
        first_moves_[i] = current_move;
    }

    else if (current_depth == max_depth_ - 1) {
        node_count_per_first_move_[first_move_idx]++;
    }

    node_count_[current_depth + 1]++;

    if (current_move.is_any_capture()) {
        capture_count_[current_depth + 1]++;
    }

    if (current_move.is_any_promo()) {
        promo_count_[current_depth + 1]++;
    }

    if (current_move.is_any_castle()) {
        casle_count_[current_depth + 1]++;
    }

    if (current_move.is_ep_capture()) {
        ep_capture_count_[current_depth + 1]++;
    }

    // FIXME: This is temporary
    // if (board_.isDeadPosition() || 49 >= 50)
    // {
    //     unmake_move(current_move, is_maximizer, current_depth);
    //     return;
    // }
    ret_flag = false;
}

} // namespace engine