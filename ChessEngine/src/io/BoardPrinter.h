#pragma once

#include "model/position/Board.h"

namespace io {

class BoardPrinter {

public:
    BoardPrinter(const model::Bitboards& bitboards);
    void fillBoard(const model::Bitboards& bitboards);
    void printBoard() const;

private:
    bool isValidPiece(model::Piece::Type piece_type);

    std::vector<std::vector<char>> _board;
};

} // namespace io