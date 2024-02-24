#include "Searcher.h"

namespace search {
    Searcher::Searcher() : _chessBoard(game::ChessBoard()),
                           _moveGenerator(game::MoveGenerator(_chessBoard)),
                           _evaluator(evaluation::Evaluator(_chessBoard)) {}
}
