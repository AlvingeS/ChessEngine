#include "engine/perft/perft.h"

#include "model/constants.h"
#include "io/board_printer.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(int maxDepth)
    : board_(),
      bbs_(board_.bitboards),
      piece_map_(board_.piece_map),
      occupancy_masks_(board_.occupancy_masks),
      z_hasher_(board_.z_hasher),
      search_memory_(maxDepth),
      move_maker_(board_),
      move_retractor_(board_),
      move_generator_(board_, move_maker_, move_retractor_),
      eval_(board_),
      perft_data_(maxDepth),
      num_move_gen_calls_(0),
      total_nodes_(0)
{
    // For any non-uniform initialization:
    last_captured_pieces_.resize(max_depth_, model::Piece::Type::EMPTY);
    move_lists_.resize(max_depth_);
    no_captures_or_pawn_moves_counts_.resize(max_depth_, 0);
}


void perft::gen_moves(
    bool is_w,
    int current_depth,
    bitmask ep_target_mask,
    unsigned char castle_rights) 
{
    move_generator_.gen_moves(is_w, move_lists_[current_depth], ep_target_mask, castle_rights);
}

logic::MoveResult perft::make_move(
    model::Move move,
    bool is_w) 
{
    return move_maker_.make_move(move, is_w);
}

void perft::unmake_move(
    model::Move move,
    bool is_w,
    logic::MoveResult previousMoveResult)
{
    move_retractor_.unmake_move(move, is_w, previousMoveResult);
}

void perft::debug_print(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(bbs_);
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
//         if (movelist.moves[i].getFlag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool perft::too_many_pieces_on_board() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (piece_map_.get_piece_type_at(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool perft::check_condition(
    int current_depth,
    bool isMaximizer,
    int first_move_idx, 
    model::Move currentMove, 
    model::Move lastMove, 
    bool verbose, 
    size_t i) const
{
    // return not board_.getKingMoved(false);
    // return too_many_pieces_on_board();
    // return first_move_idx == 19 && currentMove.is_any_capture();
    // return currentMove.get_from_sq() == 12 && currentMove.get_to_sq() == 12;
    // return current_depth == 3 && first_move_idx == 0 && currentMove.get_from_sq() == 34 && currentMove.get_to_sq() == 27;
    // return currentMove.is_any_capture();
    // return true;
    return false;
    // return diff_between_occupancy_masks();
    // return current_depth == 2 && first_move_idx == 0 && isMaximizer == true && currentMove.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void perft::minimax(
    int current_depth, 
    bool isMaximizer, 
    int first_move_idx, 
    bool recPerftStats, 
    const model::Move& lastMove, 
    bool verbose)
{        
    if (current_depth == max_depth_)
        return;

    gen_moves(
        isMaximizer, 
        current_depth,
        search_memory_.get_ep_target_mask_at_depth(current_depth),
        search_memory_.get_castle_rights_at_depth(current_depth)
    );

    num_move_gen_calls_++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = move_lists_[current_depth].get_move_at(i);

        if (currentMove.get_move() == 0)
            break;

        if (check_condition(
            current_depth, 
            isMaximizer, 
            first_move_idx, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debug_print(verbose);
            int x = 4;
        }

        // Make the move and check if we are in any way left in check
        logic::MoveResult move_result = make_move(currentMove, isMaximizer);

        if (check_condition(
            current_depth, 
            isMaximizer, 
            first_move_idx, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debug_print(verbose);
            int x = 4;
        }

        if (move_generator_.in_check(isMaximizer)) {
            numIllegalMoves++;
            unmake_move(currentMove, isMaximizer, move_result);

            if (check_condition(
                current_depth, 
                isMaximizer, 
                first_move_idx, 
                currentMove, 
                lastMove, 
                verbose, 
                i)) 
            {
                debug_print(verbose);
                int x = 4;
            }

            if (numIllegalMoves == i + 1 && move_lists_[current_depth].get_move_at(i + 1).get_move() == 0) {
                bool wasInCheckBeforeMove = move_generator_.in_check(isMaximizer);

                if (wasInCheckBeforeMove) {
                    perft_data_.increase_checkmate_count_at(current_depth);
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights
        search_memory_.set_castle_rights(
            current_depth,
            currentMove, 
            isMaximizer, 
            piece_map_.get_piece_type_at(currentMove.get_to_sq())
        );

        if (recPerftStats) {
            bool ret_flag;
            
            record_perft_stats(
                isMaximizer, 
                current_depth, 
                first_move_idx, 
                i, 
                currentMove, 
                ret_flag
            );
            
            if (ret_flag)
                return;
        }

        minimax(
            current_depth + 1, 
            !isMaximizer, 
            first_move_idx, 
            recPerftStats, 
            currentMove, 
            verbose
        );

        unmake_move(currentMove, isMaximizer, move_result);
        search_memory_.unset_castle_rights(current_depth);
        
        if (check_condition(
            current_depth, 
            isMaximizer, 
            first_move_idx, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debug_print(verbose);
            int x = 4;
        }
    }

    return;
}

void perft::record_perft_stats(
    bool isMaximizer, 
    int current_depth, 
    int &first_move_idx, 
    size_t i, 
    const model::Move& currentMove, 
    bool &ret_flag) 
{
    ret_flag = true;
    if (move_generator_.in_check(!isMaximizer)) {
        perft_data_.increase_check_count_at(current_depth + 1);
    }

    if (current_depth == 0) {
        first_move_idx = i;
        perft_data_.set_first_move_at(first_move_idx, currentMove);
    } else if (current_depth == max_depth_ - 1) {
        perft_data_.increase_node_count_per_first_move_at(first_move_idx);
    }

    perft_data_.increase_node_count_at(current_depth + 1);

    if (currentMove.is_any_capture()) {
        perft_data_.increase_capture_count_at(current_depth + 1);
    }

    if (currentMove.is_any_promo()) {
        perft_data_.increase_promo_count_at(current_depth + 1);
    }

    if (currentMove.is_any_castle()) {
        perft_data_.increase_castle_count_at(current_depth + 1);
    }

    if (currentMove.is_ep_capture()) {
        perft_data_.increase_ep_capture_count_at(current_depth + 1);
    }

    // FIXME: This is temporary
    // if (board_.isDeadPosition() || 49 >= 50)
    // {
    //     unmake_move(currentMove, isMaximizer, current_depth);
    //     return;
    // }
    ret_flag = false;
}

} // namespace engine