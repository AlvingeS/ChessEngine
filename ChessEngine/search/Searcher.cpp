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
        
        _nodeCountPerFirstMove.resize(218);
        _firstMoves.resize(218);

        for (int i = 0; i < 218; i++) {
            _nodeCountPerFirstMove[i] = 0;
            _firstMoves[i] = game::Move();
        }

        _nodeCount.resize(20);
        _captureCount.resize(20);
        _epCaptureCount.resize(20);
        _castlingCount.resize(20);
        _promotionCount.resize(20);
        _checkCount.resize(20);
        _checkmateCount.resize(20);
        _moveStack.resize(20);

        for (int i = 0; i < 20; i++) {
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

        // if (_board.getPieceTypeAtIndex(32) == _board.getPieceTypeAtIndex(38) && _board.getPieceTypeAtIndex(32) != game::PieceType::EMPTY) {
        //     std::cout << "Illegal move: " << std::endl;
        // }
    }

    void Searcher::unmakeMove(game::Move move, bool isWhite) {
        _board.unmakeMove(move, isWhite);
        _moveGenerator.updateGameStateBitmasks();

        // if (_board.getPieceTypeAtIndex(32) == _board.getPieceTypeAtIndex(38) && _board.getPieceTypeAtIndex(32) != game::PieceType::EMPTY) {
        //     std::cout << "Illegal move: " << std::endl;
        // }
    }

    void Searcher::debugPrint(bool verbose, bool condition) {
        if (verbose && condition) {
            game::BoardPrinter boardPrinter = game::BoardPrinter(_board.getBitboards());
            boardPrinter.printBoard();
        }
    }

    // Helper function to check if there are any castling moves in the movelist
    bool hasTwoCastlingMove(MoveList& moveList) {
        int count = 0;

        for (size_t i = 0; i < moveList.numMoves; i++) {
            if (moveList.moves[i].isAnyCastle()) {
                count++;
            }
        }

        return count == 2;
    }

    bool noKingSideCastling(MoveList& moveList) {
        for (size_t i = 0; i < moveList.numMoves; i++) {
            if (moveList.moves[i].getFlag() == 3) {
                return false;
            }
        }

        return true;
    }

    bool noQueenSideCastling(MoveList& moveList) {
        for (size_t i = 0; i < moveList.numMoves; i++) {
            if (moveList.moves[i].getFlag() == 2) {
                return false;
            }
        }

        return true;
    }

    // TODO: Implement draw by repetition after implementing zobrist hashing
    MoveScore Searcher::minimax(int currentDepth, bool isMaximizer, int firstMoveIndex, bool verbose) {
        // int multiplier = isMaximizer ? 1 : -1;
        
        if (currentDepth == _maxDepth) {
            return MoveScore{game::Move(), 0};
        }

        MoveScore bestEval = {game::Move(), 0};
        MoveList moveList = genMoves(isMaximizer);
        _numMoveGenCalls++;
        
        size_t numIllegalMoves = 0;

        for (size_t i = 0; i < moveList.numMoves; i++) {
            game::Move currentMove = moveList.moves[i];

            // bool condition = currentMove.getBitIndexFrom() == 32 && currentMove.getBitIndexTo() == 38;
            // bool condition = currentDepth == 0 && i == 1;
            // bool condition = _board.getSquaresLookup()[59] == game::PieceType::B_KING;
            // bool condition = currentMove.getMove() == 12288 && not isMaximizer;
            // bool condition = verbose && !hasTwoCastlingMove(moveList) && currentDepth == 1;
            // bool condition = verbose && currentMove.getMove() == 391 && currentDepth == 0;

            // Make the move and check if we are in any way left in check
            // debugPrint(verbose, condition);
            makeMove(currentMove, isMaximizer);
            // debugPrint(verbose, condition);

            game::PieceType lastCapturedPiece = _board.getLastCapturedPiece();

            if (_moveGenerator.isInCheck(isMaximizer)) {
                numIllegalMoves++;
                unmakeMove(currentMove, isMaximizer);
                // debugPrint(verbose, condition);

                if (numIllegalMoves == moveList.numMoves) {
                    bool wasInCheckBeforeMove = _moveGenerator.isInCheck(isMaximizer);

                    if (wasInCheckBeforeMove) {
                        _checkmateCount[currentDepth]++;
                        return {currentMove, 0};
                    } else {
                        return {currentMove, 0.0f};
                    }
                }

                continue;
            }

            if (_moveGenerator.isInCheck(!isMaximizer)) {
                _checkCount[currentDepth + 1]++;
            }

            if (currentDepth == 0) {
                firstMoveIndex = i;
                _firstMoves[i] = currentMove;
            } else {
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

            if (_moveGenerator.isDeadPosition() || _board.getNoCaptureOrPawnMoveCount() >= 50) {
                unmakeMove(currentMove, isMaximizer);
                return {currentMove, 0.0f};
            }

            MoveScore eval = minimax(currentDepth + 1, !isMaximizer, firstMoveIndex, verbose);
            
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
            // debugPrint(verbose, condition);
        }

        return bestEval;
    }
}