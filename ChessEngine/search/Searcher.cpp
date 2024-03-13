#include "Searcher.h"
#include <cstdlib>
#include "ChessEngine/game/BoardPrinter.h"

namespace search {
    Searcher::Searcher(int maxDepth) : _board(game::ChessBoard()),
                                       _moveGenerator(game::MoveGenerator(_board)),
                                       _evaluator(evaluation::Evaluator(_board)),
                                       _maxDepth(maxDepth) {
        
        _numMoveGenCalls = 0;
        _totalNodes = 0;
        
        _nodeCount.resize(maxDepth + 1);
        _moveStack.resize(maxDepth);

        for (int i = 0; i < maxDepth; i++) {
            _nodeCount[i + 1] = 0;
            _moveStack[i].resize(game::MoveGenerator::MAX_LEGAL_MOVES);
        }

        _nodeCount[0] = 1;
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

    // TODO: Implement draw by repetition after implementing zobrist hashing
    MoveScore Searcher::minimax(int currentDepth, bool isMaximizer) {
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
            makeMove(currentMove, isMaximizer);
            game::PieceType lastCapturedPiece = _board.getLastCapturedPiece();

            if (_moveGenerator.isInCheck(isMaximizer)) {
                unmakeMove(currentMove, isMaximizer);
                numIllegalMoves++;

                if (numIllegalMoves == moveList.numMoves) {
                    bool wasInCheckBeforeMove = _moveGenerator.isInCheck(true);

                    if (wasInCheckBeforeMove) {
                        return {currentMove, multiplier * 1000000.0f};
                    } else {
                        return {currentMove, 0.0f};
                    }
                }

                continue;
            }

            _nodeCount[currentDepth + 1]++;

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
        }

        return bestEval;
    }
}