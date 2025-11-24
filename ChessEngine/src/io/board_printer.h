#pragma once

#include "model/types.h"
#include "model/position/board.h"

#include <vector>

namespace io {

class BoardPrinter {

public:
    BoardPrinter(const model::Bitboards& bbs);
    void fill_board(const model::Bitboards& bbs);
    void print() const;

private:
    bool is_valid_piece(model::Piece::Type piece_type);

    std::vector<std::vector<char>> board_;
};

} // namespace io