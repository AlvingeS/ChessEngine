#pragma once
#include "ChessEngine/game/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/Move.h"

namespace search {
    class Searcher {
        public:
            Searcher();
            void genMoves(bool isWhite);
            void makeMove(game::Move move);
            void undoMove();
            
        private:
            game::ChessBoard _chessBoard;
            game::MoveGenerator _moveGenerator;
            evaluation::Evaluator _evaluator;

            game::Move _lastMove;
            std::vector<game::Move> _moveList;
    };
}