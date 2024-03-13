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
            Searcher(int maxDepth);
            MoveScore minimax(int current_depth, bool isMaximizer);
            MoveList genMoves(bool isWhite);
            void makeMove(game::Move move, bool isWhite);
            void unmakeMove(game::Move move, bool isWhite);
            void undoMove();
            int _numMoveGenCalls;
            int _totalNodes;
            std::vector<int> _nodeCount;

            game::ChessBoard& getBoard() {
                return _board;
            }

            game::MoveGenerator& getMoveGenerator() {
                return _moveGenerator;
            }
            
        private:
            game::ChessBoard _board;
            game::MoveGenerator _moveGenerator;
            evaluation::Evaluator _evaluator;
            int _maxDepth;

            int _pseudoLegalMovesCount;
            std::vector<std::vector<game::Move>> _moveStack;
    };
}