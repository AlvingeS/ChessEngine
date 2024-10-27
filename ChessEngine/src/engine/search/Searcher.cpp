#include "ChessEngine/src/engine/search/Searcher.h"

#include "ChessEngine/src/io/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace engine {
namespace search {

Searcher::Searcher(int maxDepth) 
    : _bitboards(representation::board::Bitboards())
    , _squaresLookup(representation::board::SquaresLookup(_bitboards))
    , _gameStateBitmasks(representation::board::GameStateBitmasks(_bitboards))
    , _searchMemory(SearchMemory(maxDepth))
    , _zHasher(representation::board::ZHasher(_squaresLookup))
    , _moveMaker(logic::makemove::MoverMaker(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveRetractor(logic::makemove::MoveMaker(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveGenerator(logic::movegen::MoveGenerator(_bitboards, _gameStateBitmasks, _moveMaker, _moveRetractor))
    , _evaluator(evaluation::Evaluator(_bitboards))
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;
    
    _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
    _firstMoves.resize(MAX_LEGAL_MOVES);

    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = representation::move::Move();
    }

    _lastCapturedPieces.resize(_maxDepth);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = representation::board::PieceType::EMPTY;
        _movelists[i] = representation::move::Movelist();
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

long Searcher::sumNodesToDepth(int depth) const {
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void Searcher::genMoves(
    bool isWhite,
    int currentDepth,
    unsigned char castlingRights) 
{
    _moveGenerator.genMoves(isWhite, _movelists[currentDepth], currentDepth, castlingRights);
}

void Searcher::makeMove(
    representation::move::Move move,
    bool isWhite,
    int currentdepth) 
{
    _moveMaker.makeMove(move, isWhite, currentdepth);
}

void Searcher::unmakeMove(
    representation::move::Move move,
    bool isWhite,
    int currentDepth)
{
    _moveRetractor.unmakeMove(move, isWhite, currentDepth);
}

void Searcher::debugPrint(bool verbose) const
{
    if (verbose) {
        utils::BoardPrinter boardPrinter = utils::BoardPrinter(_bitboards);
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

bool Searcher::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (_squaresLookup.getPieceTypeAtIndex(i) != representation::board::PieceType::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool Searcher::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    representation::move::Move currentMove, 
    representation::move::Move lastMove, 
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
    // return diffBetweenGameStateBitmasks();
    // return currentDepth == 2 && firstMoveIndex == 0 && isMaximizer == true && currentMove.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void Searcher::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    const representation::move::Move& lastMove, 
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

    for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
        representation::move::Move currentMove = _movelists[currentDepth].getMoveAt(i);

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
                    _checkmateCount[currentDepth]++;
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
            _squaresLookup.getPieceTypeAtIndex(currentMove.getBitIndexTo())
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

void Searcher::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const representation::move::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGenerator.isInCheck(!isMaximizer))
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

    if (currentMove.isAnyCapture()) {
        _captureCount[currentDepth + 1]++;
    }

    if (currentMove.isAnyPromo()) {
        _promotionCount[currentDepth + 1]++;
    }

    if (currentMove.isAnyCastle()) {
        _castlingCount[currentDepth + 1]++;
    }

    if (currentMove.isEpCapture()) {
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

} // namespace search
} // namespace engine