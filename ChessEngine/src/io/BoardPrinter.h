#pragma once

#include "ChessEngine/src/model/board/PieceType.h"
#include "ChessEngine/src/model/board/Bitboards.h"

namespace io {

class BoardPrinter {

public:
    // Public member functions
    BoardPrinter(const model::Bitboards& bitboards);
    void fillBoard(const model::Bitboards& bitboards);
    void printBoard() const;

private:
    // Private member variables
    std::vector<std::vector<char>> _board;

    // Private member functions
    bool isValidPiece(model::PieceType piece_type);
    
};

} // namespace io