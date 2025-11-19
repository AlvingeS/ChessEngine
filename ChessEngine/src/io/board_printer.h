#pragma once

#include "model/types.h"
#include "model/position/board.h"

#include <vector>

namespace io {

class BoardPrinter {

public:
    BoardPrinter(const model::Bitboards& bitboards);
    void fillBoard(const model::Bitboards& bitboards);
    void printBoard() const;

private:
    bool isValidPiece(model::Piece::Type piece_type);

    std::vector<std::vector<char>> board_;
};

} // namespace io