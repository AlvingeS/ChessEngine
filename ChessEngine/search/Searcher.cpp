#include "Searcher.h"

namespace search {
    Searcher::Searcher() : _board(game::ChessBoard()),
                           _moveGenerator(game::MoveGenerator(_board)),
                           _evaluator(evaluation::Evaluator(_board)) {}

    void Searcher::makeMove(game::Move move, bool isWhite) {
        _board.makeMove(move, isWhite);
    }
}
