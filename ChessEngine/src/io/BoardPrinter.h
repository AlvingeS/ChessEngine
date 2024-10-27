#pragma once

#include "ChessEngine/src/representation/board/PieceType.h"
#include "ChessEngine/src/representation/board/Bitboards.h"

namespace io {

class BoardPrinter {

public:
    // Public member functions
    BoardPrinter(const representation::board::Bitboards& bitboards);
    void fillBoard(const representation::board::Bitboards& bitboards);
    void printBoard() const;

private:
    // Private member variables
    std::vector<std::vector<char>> _board;

    // Private member functions
    bool isValidPiece(representation::board::PieceType piece_type);
    
};

} // namespace io