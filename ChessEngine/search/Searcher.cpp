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

    void Searcher::genMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _moveGenerator.resetMoveIndex();
        _moveGenerator.genMoves(isWhite, moveList);
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
    void Searcher::minimax(int currentDepth, bool isMaximizer, int firstMoveIndex, game::Move lastMove, bool verbose) {        
        if (currentDepth == _maxDepth) {
            return;
        }

        genMoves(isMaximizer, _moveLists[currentDepth]);
        _numMoveGenCalls++;
        
        size_t numIllegalMoves = 0;

        for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
            game::Move currentMove = _moveLists[currentDepth][i];

            if (currentMove.getMove() == 0) {
                break;
            }

            bool condition = verbose && currentDepth == 99;

            // Make the move and check if we are in any way left in check
            debugPrint(verbose, condition);
            makeMove(currentMove, isMaximizer);
            debugPrint(verbose, condition);

            game::PieceType lastCapturedPiece = _board.getLastCapturedPiece();

            if (_moveGenerator.isInCheck(isMaximizer)) {
                numIllegalMoves++;
                unmakeMove(currentMove, isMaximizer);

                if (numIllegalMoves == i + 1 && _moveLists[currentDepth][i + 1].getMove() == 0) {
                    bool wasInCheckBeforeMove = _moveGenerator.isInCheck(isMaximizer);

                    if (wasInCheckBeforeMove) {
                        _checkmateCount[currentDepth]++;
                    }

                    return;
                }

                continue;
            }

            bool retFlag;
            recordPerftStats(isMaximizer, currentDepth, firstMoveIndex, i, currentMove, retFlag);
            if (retFlag)
                return;

            minimax(currentDepth + 1, !isMaximizer, firstMoveIndex, currentMove, verbose);

            _board.setLastCapturedPiece(lastCapturedPiece);
            unmakeMove(currentMove, isMaximizer);
            debugPrint(verbose, condition);
            int x = 4;
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

        if (_moveGenerator.isDeadPosition() || _board.getNoCaptureOrPawnMoveCount() >= 50)
        {
            unmakeMove(currentMove, isMaximizer);
            return;
        }
        retFlag = false;
    }
}