#include "ChessEngine/src/ponder/perft/Searcher.h"

#include "ChessEngine/src/utils/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace ponder {
namespace perft {

Searcher::Searcher(
    int maxDepth,
    game::board::Bitboards& bitboards,
    game::board::SquaresLookup& squaresLookup,
    game::board::GameStateBitmasks& gameStateBitmasks,
    SearchMemory& searchMemory,
    game::board::ZHasher& zHasher,
    game::move::MoveMaker& moveMaker,
    game::move::MoveRetractor& moveRetractor,
    game::movegen::MoveGenerator& moveGenerator,
    evaluation::Evaluator& evaluator) 
    : _bitboardsRef(bitboards)
    , _squaresLookupRef(squaresLookup)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _searchMemoryRef(searchMemory) 
    , _zHasherRef(zHasher)
    , _moveMakerRef(moveMaker)
    , _moveRetractorRef(moveRetractor)
    , _moveGeneratorRef(moveGenerator)
    , _evaluatorRef(evaluator)
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;
    
    _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
    _firstMoves.resize(MAX_LEGAL_MOVES);

    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = game::move::Move();
    }

    _lastCapturedPieces.resize(_maxDepth);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = game::board::PieceType::EMPTY;
        _movelists[i] = game::movegen::Movelist();
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
    _moveGeneratorRef.genMoves(isWhite, _movelists[currentDepth], currentDepth, castlingRights);
}

void Searcher::makeMove(
    game::move::Move move,
    bool isWhite,
    int currentdepth) 
{
    _moveMakerRef.makeMove(move, isWhite, currentdepth);
}

void Searcher::unmakeMove(
    game::move::Move move,
    bool isWhite,
    int currentDepth)
{
    _moveRetractorRef.unmakeMove(move, isWhite, currentDepth);
}

void Searcher::debugPrint(bool verbose) const
{
    if (verbose) {
        utils::BoardPrinter boardPrinter = utils::BoardPrinter(_bitboardsRef);
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
        if (_squaresLookupRef.getPieceTypeAtIndex(i) != game::board::PieceType::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

void Searcher::resetNodeCounts() 
{
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

bool Searcher::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    game::move::Move currentMove, 
    game::move::Move lastMove, 
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
    const game::move::Move& lastMove, 
    bool verbose)
{        
    if (currentDepth == _maxDepth) {
        return;
    }

    genMoves(
        isMaximizer, 
        currentDepth, 
        _searchMemoryRef.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
        game::move::Move currentMove = _movelists[currentDepth].getMoveAt(i);

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

        if (_moveGeneratorRef.isInCheck(isMaximizer)) {
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
                bool wasInCheckBeforeMove = _moveGeneratorRef.isInCheck(isMaximizer);

                if (wasInCheckBeforeMove) {
                    _checkmateCount[currentDepth]++;
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights
        _searchMemoryRef.setCastlingRights(
            currentDepth,
            currentMove, 
            isMaximizer, 
            _squaresLookupRef.getPieceTypeAtIndex(currentMove.getBitIndexTo())
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
        _searchMemoryRef.unsetCastlingRights(currentDepth);
        
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
    const game::move::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGeneratorRef.isInCheck(!isMaximizer))
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
} // namespace ponder