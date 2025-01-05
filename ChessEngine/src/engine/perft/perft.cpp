#include "ChessEngine/src/engine/perft/perft.h"

#include "ChessEngine/src/io/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(int maxDepth)
    : _bitboards(),
      _pieceMap(_bitboards),
      _stateBitmasks(_bitboards),
      _zHasher(_pieceMap),
      _searchMemory(maxDepth),
      _moveMaker(_bitboards, _stateBitmasks, _pieceMap, _zHasher, _searchMemory),
      _moveRetractor(_bitboards, _stateBitmasks, _pieceMap, _zHasher, _searchMemory),
      _moveGenerator(_bitboards, _stateBitmasks, _moveMaker, _moveRetractor),
      _evaluator(_bitboards),
      _perftData(maxDepth),
      _numMoveGenCalls(0),
      _totalNodes(0)
{
    // For any non-uniform initialization:
    _lastCapturedPieces.resize(_maxDepth, model::PieceType::EMPTY);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth, 0);
}


void perft::genMoves(
    bool isWhite,
    int currentDepth,
    unsigned char castlingRights) 
{
    _moveGenerator.genMoves(isWhite, _movelists[currentDepth], currentDepth, castlingRights);
}

void perft::makeMove(
    model::Move move,
    bool isWhite,
    int currentdepth) 
{
    _moveMaker.makeMove(move, isWhite, currentdepth);
}

void perft::unmakeMove(
    model::Move move,
    bool isWhite,
    int currentDepth)
{
    _moveRetractor.unmakeMove(move, isWhite, currentDepth);
}

void perft::debugPrint(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(_bitboards);
        boardPrinter.printBoard();
    }
}

// Helper function to check if there are any castling moves in the movelist
// bool hasTwoCastlingMove(MoveList& movelist) {
//     int count = 0;

//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].isAnyCastle()) {
//             count++;
//         }
//     }

//     return count == 2;
// }

// bool noKingSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].getFlag() == 3) {
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
        if (_pieceMap.getPieceTypeAtIndex(i) != model::PieceType::EMPTY) {
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
    // return firstMoveIndex == 19 && currentMove.isAnyCapture();
    // return currentMove.getBitIndexFrom() == 12 && currentMove.getBitIndexTo() == 12;
    // return currentDepth == 3 && firstMoveIndex == 0 && currentMove.getBitIndexFrom() == 34 && currentMove.getBitIndexTo() == 27;
    // return currentMove.isAnyCapture();
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
    if (currentDepth == _maxDepth) {
        return;
    }

    genMoves(
        isMaximizer, 
        currentDepth, 
        _searchMemory.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < logic::MoveGenerator::MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = _movelists[currentDepth].getMoveAt(i);

        if (currentMove.getMove() == 0) {
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
        makeMove(currentMove, isMaximizer, currentDepth);

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

        if (_moveGenerator.isInCheck(isMaximizer)) {
            numIllegalMoves++;
            unmakeMove(currentMove, isMaximizer, currentDepth);

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

            if (numIllegalMoves == i + 1 && _movelists[currentDepth].getMoveAt(i + 1).getMove() == 0) {
                bool wasInCheckBeforeMove = _moveGenerator.isInCheck(isMaximizer);

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
            _pieceMap.getPieceTypeAtIndex(currentMove.getBitIndexTo())
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

        unmakeMove(currentMove, isMaximizer, currentDepth);
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
    if (_moveGenerator.isInCheck(!isMaximizer)) {
        _perftData.increaseCheckCountAt(currentDepth + 1);
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _perftData.setFirstMoveAt(firstMoveIndex, currentMove);
    } else if (currentDepth == _maxDepth - 1) {
        _perftData.increaseNodeCountPerFirstMoveAt(firstMoveIndex);
    }

    _perftData.increaseNodeCountAt(currentDepth + 1);

    if (currentMove.isAnyCapture()) {
        _perftData.increaseCaptureCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyPromo()) {
        _perftData.increasePromotionCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyCastle()) {
        _perftData.increaseCastlingCountAt(currentDepth + 1);
    }

    if (currentMove.isEpCapture()) {
        _perftData.increaseEpCaptureCountAt(currentDepth + 1);
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