#include "Searcher.h"
#include <cstdlib>
#include "ChessEngine/game/BoardPrinter.h"
#include <iostream>

namespace search {
    Searcher::Searcher(int maxDepth) : _board(game::ChessBoard()),
                                       _searchMemory(SearchMemory(maxDepth)),
                                       _moveMaker(_board, _searchMemory),
                                       _moveGenerator(movegen::MoveGenerator(_board, _moveMaker)),
                                       _evaluator(evaluation::Evaluator(_board)),
                                       _maxDepth(maxDepth) {
        
        _numMoveGenCalls = 0;
        _totalNodes = 0;
        
        _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
        _firstMoves.resize(MAX_LEGAL_MOVES);

        for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
            _nodeCountPerFirstMove[i] = 0;
            _firstMoves[i] = game::Move();
        }

        _lastCapturedPieces.resize(_maxDepth);
        _moveLists.resize(_maxDepth);
        _noCapturedOrPawnMoveCounts.resize(_maxDepth);

        for (int i = 0; i < _maxDepth; i++) {
            _lastCapturedPieces[i] = game::PieceType::EMPTY;
            _moveLists[i] = std::vector<game::Move>(MAX_LEGAL_MOVES);
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

    int Searcher::sumNodesToDepth(int depth) {
        int sum = 0;

        for (int i = 1; i <= depth; i++) {
            sum += _nodeCount[i];
        }

        return sum;
    }

    void Searcher::genMoves(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, unsigned char castlingRights) {
        _moveGenerator.resetMoveIndex();
        _moveGenerator.genMoves(isWhite, moveList, currentDepth, castlingRights);
    }

    void Searcher::makeMove(game::Move move, bool isWhite, int currentdepth) {
        _moveMaker.makeMove(move, isWhite, currentdepth);
    }

    void Searcher::unmakeMove(game::Move move, bool isWhite, int currentDepth) {
        _moveMaker.unmakeMove(move, isWhite, currentDepth);
    }

    void Searcher::debugPrint(bool verbose) {
        if (verbose) {
            game::BoardPrinter boardPrinter = game::BoardPrinter(_board.getBitboards());
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

    bool Searcher::checkCondition(int currentDepth, bool isMaximizer, int firstMoveIndex, game::Move currentMove, game::Move lastMove, bool verbose, size_t i) {
        // return not _board.getKingMoved(false);
        return currentDepth == 2 && currentMove.getFlag() == 12;
    }

    // TODO: Implement draw by repetition after implementing zobrist hashing
    void Searcher::minimax(int currentDepth, bool isMaximizer, int firstMoveIndex, bool recPerftStats, game::Move lastMove, bool verbose) {        
        if (currentDepth == _maxDepth) {
            return;
        }

        genMoves(isMaximizer, _moveLists[currentDepth], currentDepth, _searchMemory.getCastlingRightsAtDepth(currentDepth));
        _numMoveGenCalls++;
        
        size_t numIllegalMoves = 0;

        for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
            game::Move currentMove = _moveLists[currentDepth][i];

            if (currentMove.getMove() == 0) {
                break;
            }

            // if (checkCondition(currentDepth, isMaximizer, firstMoveIndex, currentMove, lastMove, verbose, i)) {
            //     debugPrint(verbose);
            //     int x = 4;
            // }

            // Make the move and check if we are in any way left in check
            makeMove(currentMove, isMaximizer, currentDepth);

            // if (checkCondition(currentDepth, isMaximizer, firstMoveIndex, currentMove, lastMove, verbose, i)) {
            //     debugPrint(verbose);
            //     int x = 4;
            // }

            if (_moveGenerator.isInCheck(isMaximizer)) {
                numIllegalMoves++;
                unmakeMove(currentMove, isMaximizer, currentDepth);

                // if (checkCondition(currentDepth, isMaximizer, firstMoveIndex, currentMove, lastMove, verbose, i)) {
                //     debugPrint(verbose);
                //     int x = 4;
                // }

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
            _searchMemory.setCastlingRights(currentDepth, currentMove, isMaximizer, _board.getPieceTypeAtIndex(currentMove.getBitIndexTo()));


            if (recPerftStats) {
                bool retFlag;
                recordPerftStats(isMaximizer, currentDepth, firstMoveIndex, i, currentMove, retFlag);
                if (retFlag)
                    return;
            }

            minimax(currentDepth + 1, !isMaximizer, firstMoveIndex, recPerftStats, currentMove, verbose);

            unmakeMove(currentMove, isMaximizer, currentDepth);
            _searchMemory.unsetCastlingRights(currentDepth);
            
            // if (checkCondition(currentDepth, isMaximizer, firstMoveIndex, currentMove, lastMove, verbose, i)) {
            //     debugPrint(verbose);
            //     int x = 4;
            // }
        }

        return;
    }

    void Searcher::recordPerftStats(bool isMaximizer, int currentDepth, int &firstMoveIndex, size_t i, game::Move &currentMove, bool &retFlag) {
        retFlag = true;
        if (_moveGenerator.isInCheck(!isMaximizer))
        {
            _checkCount[currentDepth + 1]++;
        }

        if (currentDepth == 0)
        {
            firstMoveIndex = i;
            _firstMoves[i] = currentMove;
        }
        else if (currentDepth == _maxDepth - 1)
        {
            _nodeCountPerFirstMove[firstMoveIndex]++;
        }

        _nodeCount[currentDepth + 1]++;

        if (currentMove.isAnyCapture())
        {
            _captureCount[currentDepth + 1]++;
        }

        if (currentMove.isAnyPromo())
        {
            _promotionCount[currentDepth + 1]++;
        }

        if (currentMove.isAnyCastle())
        {
            _castlingCount[currentDepth + 1]++;
        }

        if (currentMove.isEpCapture())
        {
            _epCaptureCount[currentDepth + 1]++;
        }

        if (_board.isDeadPosition() || _board.getNoCaptureOrPawnMoveCount() >= 50)
        {
            unmakeMove(currentMove, isMaximizer, currentDepth);
            return;
        }
        retFlag = false;
    }
}