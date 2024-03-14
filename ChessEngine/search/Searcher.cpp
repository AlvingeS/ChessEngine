#include "Searcher.h"
#include <cstdlib>
#include "ChessEngine/game/BoardPrinter.h"
#include <iostream>

namespace search {
    Searcher::Searcher(int maxDepth) : _board(game::ChessBoard()),
                                       _moveGenerator(game::MoveGenerator(_board)),
                                       _evaluator(evaluation::Evaluator(_board)),
                                       _maxDepth(maxDepth) {
        
        _numMoveGenCalls = 0;
        _totalNodes = 0;
        
        _nodeCount.resize(maxDepth + 1);
        _captureCount.resize(maxDepth + 1);
        _epCaptureCount.resize(maxDepth + 1);
        _castlingCount.resize(maxDepth + 1);
        _promotionCount.resize(maxDepth + 1);
        _checkCount.resize(maxDepth + 1);
        _checkmateCount.resize(maxDepth + 1);
        _moveStack.resize(maxDepth);

        for (int i = 0; i < maxDepth; i++) {
            _nodeCount[i + 1] = 0;
            _captureCount[i + 1] = 0;
            _epCaptureCount[i + 1] = 0;
            _castlingCount[i + 1] = 0;
            _promotionCount[i + 1] = 0;
            _checkCount[i + 1] = 0;
            _checkmateCount[i + 1] = 0;
            _moveStack[i].resize(game::MoveGenerator::MAX_LEGAL_MOVES);
        }

        _nodeCount[0] = 1;
        _captureCount[0] = 0;
        _epCaptureCount[0] = 0;
        _castlingCount[0] = 0;
        _promotionCount[0] = 0;
        _checkCount[0] = 0;
        _checkmateCount[0] = 0;
    }

    MoveList Searcher::genMoves(bool isWhite) {
        _moveGenerator.resetMoveIndex();
        _moveGenerator.genMoves(isWhite);
        return MoveList{_moveGenerator.getMoves(), _moveGenerator.getMoveIndex()};
    }

    void Searcher::makeMove(game::Move move, bool isWhite) {
        _board.makeMove(move, isWhite);
        _moveGenerator.updateGameStateBitmasks();
    }

    void Searcher::unmakeMove(game::Move move, bool isWhite) {
        _board.unmakeMove(move, isWhite);
        _moveGenerator.updateGameStateBitmasks();
    }

    void Searcher::debugPrint(bool verbose, int currentDepth, int debugDepth) {
        if (verbose && currentDepth == debugDepth) {
            game::BoardPrinter boardPrinter = game::BoardPrinter(_board.getBitboards());
            boardPrinter.printBoard();
        }
    }

    // TODO: Implement draw by repetition after implementing zobrist hashing
    MoveScore Searcher::minimax(int currentDepth, bool isMaximizer, bool verbose) {
        int multiplier = isMaximizer ? 1 : -1;
        
        if (currentDepth == _maxDepth) {
            return MoveScore{game::Move(), multiplier * _evaluator.evaluate()};
        }

        MoveScore bestEval = {game::Move(), -multiplier * 1000000.0f};
        MoveList moveList = genMoves(isMaximizer);
        _numMoveGenCalls++;
        
        size_t numIllegalMoves = 0;

        for (size_t i = 0; i < moveList.numMoves; i++) {
            game::Move currentMove = moveList.moves[i];

            // Make the move and check if we are in any way left in check
            debugPrint(verbose, currentDepth, 0);
            makeMove(currentMove, isMaximizer);
            debugPrint(verbose, currentDepth, 0);

            game::PieceType lastCapturedPiece = _board.getLastCapturedPiece();

            if (_moveGenerator.isInCheck(isMaximizer)) {
                unmakeMove(currentMove, isMaximizer);
                numIllegalMoves++;

                if (numIllegalMoves == moveList.numMoves) {
                    bool wasInCheckBeforeMove = _moveGenerator.isInCheck(true);

                    if (wasInCheckBeforeMove) {
                        _checkmateCount[currentDepth + 1]++;
                        return {currentMove, multiplier * 1000000.0f};
                    } else {
                        return {currentMove, 0.0f};
                    }
                }

                continue;
            }

            if (_moveGenerator.isInCheck(!isMaximizer)) {
                _checkCount[currentDepth + 1]++;
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

            if (_moveGenerator.isDeadPosition() || _board.getNoCaptureOrPawnMoveCount() >= 50) {
                unmakeMove(currentMove, isMaximizer);
                return {currentMove, 0.0f};
            }

            MoveScore eval = minimax(currentDepth + 1, !isMaximizer);
            
            if (isMaximizer) {
                if (eval.score > bestEval.score) {
                    bestEval = eval;
                }
            } else {
                if (eval.score < bestEval.score) {
                    bestEval = eval;
                }
            }

            _board.setLastCapturedPiece(lastCapturedPiece);
            unmakeMove(currentMove, isMaximizer);
            debugPrint(verbose, currentDepth, 0);
        }

        return bestEval;
    }
}