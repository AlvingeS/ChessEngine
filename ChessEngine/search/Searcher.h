#pragma once
#include "ChessEngine/game/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/Move.h"

namespace search {

    struct MoveScore {
        game::Move move;
        float score;
    };

    struct MoveList {
        std::vector<game::Move> moves;
        size_t numMoves;
    };

    class Searcher {
        public:

            static constexpr int MAX_LEGAL_MOVES = 218;

            Searcher(int maxDepth);
            void minimax(int current_depth, bool isMaximizer, int firstMoveIndex, game::Move lastMove = game::Move(), bool verbose = true);
            void genMoves(bool isWhite, std::vector<game::Move>& moveList);
            void makeMove(game::Move move, bool isWhite);
            void unmakeMove(game::Move move, bool isWhite);
            void undoMove();
            int _numMoveGenCalls;
            int _totalNodes;
            std::vector<int> _nodeCountPerFirstMove;
            std::vector<game::Move> _firstMoves;
            std::vector<int> _nodeCount;
            std::vector<int> _captureCount;
            std::vector<int> _epCaptureCount;
            std::vector<int> _castlingCount;
            std::vector<int> _promotionCount;
            std::vector<int> _checkCount;
            std::vector<int> _checkmateCount;
            void debugPrint(bool verbose, bool condition);

            void setMaxDepth(int maxDepth) {
                _maxDepth = maxDepth;
            }

            int getMaxDepth() {
                return _maxDepth;
            }

            game::ChessBoard& getBoard() {
                return _board;
            }

            game::MoveGenerator& getMoveGenerator() {
                return _moveGenerator;
            }

            void setBoardFromFen(std::string fen) {
                _board.setBoardFromFen(fen);
                _moveGenerator.updateGameStateBitmasks();
            }
            
        private:
            game::ChessBoard _board;
            game::MoveGenerator _moveGenerator;
            evaluation::Evaluator _evaluator;
            int _maxDepth;

            int _pseudoLegalMovesCount;
            std::vector<std::vector<game::Move>> _moveLists;
            std::vector<game::PieceType> _lastCapturedPieces;
            std::vector<int> _noCapturedOrPawnMoveCounts; 
    };
}