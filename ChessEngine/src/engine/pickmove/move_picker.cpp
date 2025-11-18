#include "engine/pickmove/move_picker.h"

#include "io/board_printer.h"
#include "model/constants.h"

#include <cstdlib>
#include <iostream>

namespace engine {

MovePicker::MovePicker(int maxDepth) 
    : _board()
    , bitboards_(_board.bitboards)
    , piece_map_(_board.piece_map)
    , state_bitmasks_(_board.state_bitmasks)
    , _zHasher(_board.z_hasher)
    , _searchMemory(SearchMemory(maxDepth))
    , move_maker_(logic::MoveMaker(_board))
    , move_retractor_(logic::MoveRetractor(_board))
    , _moveGenerator(logic::MoveGenerator(_board, move_maker_, move_retractor_))
    , _evaluator(logic::Evaluator(_board))
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;
    
    _nodeCountPerFirstMove.resize(constants::MAX_LEGAL_MOVES);
    _firstMoves.resize(constants::MAX_LEGAL_MOVES);

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = model::Move();
    }

    _lastCapturedPieces.resize(_maxDepth);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = model::Piece::Type::EMPTY;
        _movelists[i] = model::Movelist();
        _noCapturedOrPawnMoveCounts[i] = 0;
    }

    _nodeCount.resize(20);
    _captureCount.resize(20);
    _epCaptureCount.resize(20);
    _castlingCount.resize(20);
    _promotionCount.resize(20);
    _checkCount.resize(20);
    _checkmateCount.resize(20);

    for (int i = 0; i < 20; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

long MovePicker::sumNodesToDepth(int depth) const {
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void MovePicker::gen_moves(
    bool is_w,
    int currentDepth,
    bitmask ep_target_mask,
    unsigned char castle_rights) 
{
    _moveGenerator.gen_moves(is_w, _movelists[currentDepth], ep_target_mask, castle_rights);
}

logic::MoveResult MovePicker::makeMove(model::Move move, bool is_w) 
{
    return move_maker_.makeMove(move, is_w);
}

void MovePicker::unmakeMove(
    model::Move move,
    bool is_w,
    logic::MoveResult previousMoveResult)
{
    move_retractor_.unmakeMove(move, is_w, previousMoveResult);
}

void MovePicker::debugPrint(bool verbose) const
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
//         if (movelist.moves[i].get_flag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool MovePicker::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (piece_map_.get_piece_type_at_index(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool MovePicker::checkCondition(
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
void MovePicker::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    const model::Move& lastMove, 
    bool verbose)
{        
    if (currentDepth == _maxDepth) {
        return;
    }

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

        if (currentMove.get_move() == 0) {
            break;
        }

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
        logic::MoveResult moveResult = makeMove(currentMove, isMaximizer);

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

        // FIXME: Move generator should not be queried for this
        if (_moveGenerator.in_check(isMaximizer)) {
            numIllegalMoves++;
            unmakeMove(currentMove, isMaximizer, moveResult);

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
                    _checkmateCount[currentDepth]++;
                }

                return;
            }

            continue;
        }

        if (currentMove.is_any_capture()) {
            _searchMemory.setLastCapturedPieceAtDepth(currentDepth, moveResult.capturedPieceType);
        }

        _searchMemory.handleEnPessantMemory(currentMove, isMaximizer, currentDepth, currentMove.get_bit_index_to());
        _searchMemory.handleNoCaptureCount(currentMove, currentDepth, moveResult.movedPieceType);

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

        unmakeMove(currentMove, isMaximizer, moveResult);
        _searchMemory.unsetCastlingRights(currentDepth);

        if (currentMove.is_double_pawn_push()) {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }
    
        if (not currentMove.is_any_capture() && (moveResult.capturedPieceType != model::Piece::Type::W_PAWN && moveResult.movedPieceType != model::Piece::Type::B_PAWN)) {
            _searchMemory.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }

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

void MovePicker::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGenerator.in_check(!isMaximizer))
    {
        _checkCount[currentDepth + 1]++;
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _firstMoves[i] = currentMove;
    }

    else if (currentDepth == _maxDepth - 1) {
        _nodeCountPerFirstMove[firstMoveIndex]++;
    }

    _nodeCount[currentDepth + 1]++;

    if (currentMove.is_any_capture()) {
        _captureCount[currentDepth + 1]++;
    }

    if (currentMove.is_any_promo()) {
        _promotionCount[currentDepth + 1]++;
    }

    if (currentMove.is_any_castle()) {
        _castlingCount[currentDepth + 1]++;
    }

    if (currentMove.is_ep_capture()) {
        _epCaptureCount[currentDepth + 1]++;
    }

    // FIXME: This is temporary
    // if (_board.isDeadPosition() || 49 >= 50)
    // {
    //     unmakeMove(currentMove, isMaximizer, currentDepth);
    //     return;
    // }
    retFlag = false;
}

} // namespace engine