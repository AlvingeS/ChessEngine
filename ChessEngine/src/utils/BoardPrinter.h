#pragma once

#include "ChessEngine/src/game/board/PieceType.h"
#include "ChessEngine/src/game/board/Bitboards.h"

namespace utils {

class BoardPrinter {

public:
    // Public member functions
    BoardPrinter(const board::Bitboards& bitboards);
    void fillBoard(const board::Bitboards& bitboards);
    void printBoard() const;

private:
    // Private member variables
    std::vector<std::vector<char>> _board;

    // Private member functions
    bool isValidPiece(board::PieceType piece_type);
    
};

} // namespace utils