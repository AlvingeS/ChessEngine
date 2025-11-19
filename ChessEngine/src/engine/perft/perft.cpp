#include "engine/perft/perft.h"

#include "model/constants.h"
#include "io/board_printer.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(int maxDepth)
    : board_(),
      bitboards_(board_.bitboards),
      piece_map_(board_.piece_map),
      state_bitmasks_(board_.state_bitmasks),
      z_hasher_(board_.z_hasher),
      search_memory_(maxDepth),
      move_maker_(board_),
      move_retractor_(board_),
      move_generator_(board_, move_maker_, move_retractor_),
      eval_(board_),
      _perftData(maxDepth),
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
    int currentDepth,
    bitmask ep_target_mask,
    unsigned char castle_rights) 
{
    move_generator_.gen_moves(is_w, move_lists_[currentDepth], ep_target_mask, castle_rights);
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
        io::BoardPrinter boardPrinter = io::BoardPrinter(bitboards_);
        boardPrinter.printBoard();
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
        if (piece_map_.get_piece_type_at_index(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool perft::check_condition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    model::Move currentMove, 
    model::Move lastMove, 
    bool verbose, 
    size_t i) const
{
    // return not board_.getKingMoved(false);
    // return too_many_pieces_on_board();
    // return firstMoveIndex == 19 && currentMove.is_any_capture();
    // return currentMove.get_bit_index_from() == 12 && currentMove.get_bit_index_to() == 12;
    // return currentDepth == 3 && firstMoveIndex == 0 && currentMove.get_bit_index_from() == 34 && currentMove.get_bit_index_to() == 27;
    // return currentMove.is_any_capture();
    // return true;
    return false;
    // return diffBetweenStateBitmasks();
    // return currentDepth == 2 && firstMoveIndex == 0 && isMaximizer == true && currentMove.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void perft::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    const model::Move& lastMove, 
    bool verbose)
{        
    if (currentDepth == max_depth_)
        return;

    gen_moves(
        isMaximizer, 
        currentDepth,
        search_memory_.get_ep_target_at_depth(currentDepth),
        search_memory_.get_castling_rights_at_depth(currentDepth)
    );

    num_move_gen_calls_++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = move_lists_[currentDepth].get_move_at(i);

        if (currentMove.get_move() == 0)
            break;

        if (check_condition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
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
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
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
                currentDepth, 
                isMaximizer, 
                firstMoveIndex, 
                currentMove, 
                lastMove, 
                verbose, 
                i)) 
            {
                debug_print(verbose);
                int x = 4;
            }

            if (numIllegalMoves == i + 1 && move_lists_[currentDepth].get_move_at(i + 1).get_move() == 0) {
                bool wasInCheckBeforeMove = move_generator_.in_check(isMaximizer);

                if (wasInCheckBeforeMove) {
                    _perftData.increaseCheckmateCountAt(currentDepth);
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights
        search_memory_.set_castle_rights(
            currentDepth,
            currentMove, 
            isMaximizer, 
            piece_map_.get_piece_type_at_index(currentMove.get_bit_index_to())
        );

        if (recPerftStats) {
            bool ret_flag;
            
            record_perft_stats(
                isMaximizer, 
                currentDepth, 
                firstMoveIndex, 
                i, 
                currentMove, 
                ret_flag
            );
            
            if (ret_flag)
                return;
        }

        minimax(
            currentDepth + 1, 
            !isMaximizer, 
            firstMoveIndex, 
            recPerftStats, 
            currentMove, 
            verbose
        );

        unmake_move(currentMove, isMaximizer, move_result);
        search_memory_.unset_castle_rights(currentDepth);
        
        if (check_condition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
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
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &ret_flag) 
{
    ret_flag = true;
    if (move_generator_.in_check(!isMaximizer)) {
        _perftData.increaseCheckCountAt(currentDepth + 1);
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _perftData.setFirstMoveAt(firstMoveIndex, currentMove);
    } else if (currentDepth == max_depth_ - 1) {
        _perftData.increaseNodeCountPerFirstMoveAt(firstMoveIndex);
    }

    _perftData.increaseNodeCountAt(currentDepth + 1);

    if (currentMove.is_any_capture()) {
        _perftData.increaseCaptureCountAt(currentDepth + 1);
    }

    if (currentMove.is_any_promo()) {
        _perftData.increasePromotionCountAt(currentDepth + 1);
    }

    if (currentMove.is_any_castle()) {
        _perftData.increaseCastlingCountAt(currentDepth + 1);
    }

    if (currentMove.is_ep_capture()) {
        _perftData.increaseEpCaptureCountAt(currentDepth + 1);
    }

    // FIXME: This is temporary
    // if (board_.isDeadPosition() || 49 >= 50)
    // {
    //     unmake_move(currentMove, isMaximizer, currentDepth);
    //     return;
    // }
    ret_flag = false;
}

} // namespace engine