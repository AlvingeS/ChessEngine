#include "Searcher.h"

#include <cstdlib>
#include <iostream>

#include "ChessEngine/utils/BoardPrinter.h"

namespace perft {

Searcher::Searcher(int maxDepth) 
    : _bitboards(board::BitBoards())
    , _squaresLookup(board::SquaresLookup(_bitboards))
    , _gameStateBitMasks(board::GameStateBitMasks(_bitboards))
    , _searchMemory(SearchMemory(maxDepth))
    , _zHasher(board::ZHasher())
    , _bitBoardUpdater(move::BitBoardUpdater(_bitboards))
    , _bitMaskUpdater(move::BitMaskUpdater(_gameStateBitMasks))
    , _lookupUpdater(move::LookupUpdater(_squaresLookup))
    , _moveMaker(move::MoveMaker(_bitBoardUpdater, _bitMaskUpdater, _lookupUpdater, _searchMemory, _zHasher))
    , _moveGenerator(movegen::MoveGenerator(_bitboards, _gameStateBitMasks, _moveMaker))
    , _evaluator(evaluation::Evaluator(_bitboards))
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;
    
    _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
    _firstMoves.resize(MAX_LEGAL_MOVES);

    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = move::Move();
    }

    _lastCapturedPieces.resize(_maxDepth);
    _moveLists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = board::PieceType::EMPTY;
        _moveLists[i] = std::vector<move::Move>(MAX_LEGAL_MOVES);
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

long Searcher::sumNodesToDepth(int depth) {
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void Searcher::genMoves(
    bool isWhite,
    std::vector<move::Move>& moveList,
    int currentDepth,
    unsigned char castlingRights) 
{
    _moveGenerator.resetMoveIndex();
    _moveGenerator.genMoves(isWhite, moveList, currentDepth, castlingRights);
}

void Searcher::makeMove(
    move::Move move,
    bool isWhite,
    int currentdepth) 
{
    _moveMaker.makeMove(move, isWhite, currentdepth);
}

void Searcher::unmakeMove(
    move::Move move,
    bool isWhite,
    int currentDepth)
{
    _moveMaker.unmakeMove(move, isWhite, currentDepth);
}

void Searcher::debugPrint(bool verbose) 
{
    if (verbose) {
        utils::BoardPrinter boardPrinter = utils::BoardPrinter(_bitboards);
        boardPrinter.printBoard();
    }
}

// Helper function to check if there are any castling moves in the movelist
// bool hasTwoCastlingMove(MoveList& moveList) {
//     int count = 0;

//     for (size_t i = 0; i < moveList.numMoves; i++) {
//         if (moveList.moves[i].isAnyCastle()) {
//             count++;
//         }
//     }

//     return count == 2;
// }

// bool noKingSideCastling(MoveList& moveList) {
//     for (size_t i = 0; i < moveList.numMoves; i++) {
//         if (moveList.moves[i].getFlag() == 3) {
//             return false;
//         }
//     }

//     return true;
// }

// bool noQueenSideCastling(MoveList& moveList) {
//     for (size_t i = 0; i < moveList.numMoves; i++) {
//         if (moveList.moves[i].getFlag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool Searcher::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (_squaresLookup.getPieceTypeAtIndex(i) != board::PieceType::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool Searcher::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    move::Move currentMove, 
    move::Move lastMove, 
    bool verbose, 
    size_t i) 
{
    // return not _board.getKingMoved(false);
    // return tooManyPiecesOnBoard();
    // return firstMoveIndex == 19 && currentMove.isAnyCapture();
    // return currentMove.getBitIndexFrom() == 12 && currentMove.getBitIndexTo() == 12;
    // return currentDepth == 3 && firstMoveIndex == 0 && currentMove.getBitIndexFrom() == 34 && currentMove.getBitIndexTo() == 27;
    // return currentMove.isAnyCapture();
    // return true;
    // return false;
    return diffBetweenGameStateBitMasks();
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void Searcher::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    move::Move lastMove, 
    bool verbose) 
{        
    if (currentDepth == _maxDepth) {
        return;
    }

    genMoves(
        isMaximizer, 
        _moveLists[currentDepth], 
        currentDepth, 
        _searchMemory.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
        move::Move currentMove = _moveLists[currentDepth][i];

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

            if (numIllegalMoves == i + 1 && _moveLists[currentDepth][i + 1].getMove() == 0) {
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
    move::Move &currentMove, 
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

} // namespace perft