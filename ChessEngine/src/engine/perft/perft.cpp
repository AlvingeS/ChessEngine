#include "ChessEngine/src/engine/perft/perft.h"

#include "ChessEngine/src/io/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(
    int maxDepth,
    model::Bitboards& bitboards,
    model::SquaresLookup& squaresLookup,
    model::GameStateBitmasks& gameStateBitmasks,
    model::ZHasher& zHasher,
    logic::MoveMaker& moveMaker,
    logic::MoveRetractor& moveRetractor,
    logic::MoveGenerator& moveGenerator,
    engine::search::SearchMemory& searchMemory,
    logic::Evaluator& evaluator,
    perftData& perftData) 
    : _bitboardsRef(bitboards)
    , _squaresLookupRef(squaresLookup)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _searchMemoryRef(searchMemory) 
    , _zHasherRef(zHasher)
    , _moveMakerRef(moveMaker)
    , _moveRetractorRef(moveRetractor)
    , _moveGeneratorRef(moveGenerator)
    , _evaluatorRef(evaluator)
    , _perftDataRef(perftData)
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;

    _lastCapturedPieces.resize(_maxDepth);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = model::PieceType::EMPTY;
        _movelists[i] = model::Movelist();
        _noCapturedOrPawnMoveCounts[i] = 0;
    }
}

void perft::genMoves(
    bool isWhite,
    int currentDepth,
    unsigned char castlingRights) 
{
    _moveGeneratorRef.genMoves(isWhite, _movelists[currentDepth], currentDepth, castlingRights);
}

void perft::makeMove(
    model::Move move,
    bool isWhite,
    int currentdepth) 
{
    _moveMakerRef.makeMove(move, isWhite, currentdepth);
}

void perft::unmakeMove(
    model::Move move,
    bool isWhite,
    int currentDepth)
{
    _moveRetractorRef.unmakeMove(move, isWhite, currentDepth);
}

void perft::debugPrint(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(_bitboardsRef);
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
        if (_squaresLookupRef.getPieceTypeAtIndex(i) != model::PieceType::EMPTY) {
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
    // return diffBetweenGameStateBitmasks();
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
        _searchMemoryRef.getCastlingRightsAtDepth(currentDepth)
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
                    _perftDataRef.increaseCheckmateCountAt(currentDepth);
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

void perft::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGeneratorRef.isInCheck(!isMaximizer)) {
        _perftDataRef.increaseCheckCountAt(currentDepth + 1);
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _perftDataRef.setFirstMoveAt(firstMoveIndex, currentMove);
    } else if (currentDepth == _maxDepth - 1) {
        _perftDataRef.increaseNodeCountPerFirstMoveAt(firstMoveIndex);
    }

    _perftDataRef.increaseNodeCountAt(currentDepth + 1);

    if (currentMove.isAnyCapture()) {
        _perftDataRef.increaseCaptureCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyPromo()) {
        _perftDataRef.increasePromotionCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyCastle()) {
        _perftDataRef.increaseCastlingCountAt(currentDepth + 1);
    }

    if (currentMove.isEpCapture()) {
        _perftDataRef.increaseEpCaptureCountAt(currentDepth + 1);
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