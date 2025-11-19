#include "engine/perft/perft.h"

#include "model/constants.h"
#include "io/board_printer.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(int maxDepth)
    : _board(),
      bitboards_(_board.bitboards),
      piece_map_(_board.piece_map),
      state_bitmasks_(_board.state_bitmasks),
      z_hasher_(_board.z_hasher),
      _searchMemory(maxDepth),
      move_maker_(_board),
      move_retractor_(_board),
      _moveGenerator(_board, move_maker_, move_retractor_),
      _evaluator(_board),
      _perftData(maxDepth),
      _numMoveGenCalls(0),
      _totalNodes(0)
{
    // For any non-uniform initialization:
    _lastCapturedPieces.resize(_maxDepth, model::Piece::Type::EMPTY);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth, 0);
}


void perft::gen_moves(
    bool is_w,
    int currentDepth,
    bitmask ep_target_mask,
    unsigned char castle_rights) 
{
    _moveGenerator.gen_moves(is_w, _movelists[currentDepth], ep_target_mask, castle_rights);
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

void perft::debugPrint(bool verbose) const
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

bool perft::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (piece_map_.get_piece_type_at_index(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool perft::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    model::Move currentMove, 
    model::Move lastMove, 
    bool verbose, 
    size_t i) const
{
    // return not _board.getKingMoved(false);
    // return tooManyPiecesOnBoard();
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
    if (currentDepth == _maxDepth)
        return;

    gen_moves(
        isMaximizer, 
        currentDepth,
        _searchMemory.getEnPessantTargetAtDepth(currentDepth),
        _searchMemory.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = _movelists[currentDepth].get_move_at(i);

        if (currentMove.get_move() == 0)
            break;

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        // Make the move and check if we are in any way left in check
        logic::MoveResult moveResult = make_move(currentMove, isMaximizer);

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        if (_moveGenerator.in_check(isMaximizer)) {
            numIllegalMoves++;
            unmake_move(currentMove, isMaximizer, moveResult);

            if (checkCondition(
                currentDepth, 
                isMaximizer, 
                firstMoveIndex, 
                currentMove, 
                lastMove, 
                verbose, 
                i)) 
            {
                debugPrint(verbose);
                int x = 4;
            }

            if (numIllegalMoves == i + 1 && _movelists[currentDepth].get_move_at(i + 1).get_move() == 0) {
                bool wasInCheckBeforeMove = _moveGenerator.in_check(isMaximizer);

                if (wasInCheckBeforeMove) {
                    _perftData.increaseCheckmateCountAt(currentDepth);
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights
        _searchMemory.setCastlingRights(
            currentDepth,
            currentMove, 
            isMaximizer, 
            piece_map_.get_piece_type_at_index(currentMove.get_bit_index_to())
        );

        if (recPerftStats) {
            bool retFlag;
            
            recordPerftStats(
                isMaximizer, 
                currentDepth, 
                firstMoveIndex, 
                i, 
                currentMove, 
                retFlag
            );
            
            if (retFlag)
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

        unmake_move(currentMove, isMaximizer, moveResult);
        _searchMemory.unsetCastlingRights(currentDepth);
        
        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }
    }

    return;
}

void perft::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGenerator.in_check(!isMaximizer)) {
        _perftData.increaseCheckCountAt(currentDepth + 1);
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _perftData.setFirstMoveAt(firstMoveIndex, currentMove);
    } else if (currentDepth == _maxDepth - 1) {
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
    // if (_board.isDeadPosition() || 49 >= 50)
    // {
    //     unmake_move(currentMove, isMaximizer, currentDepth);
    //     return;
    // }
    retFlag = false;
}

} // namespace engine