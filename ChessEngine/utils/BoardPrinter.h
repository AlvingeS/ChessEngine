#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/PieceType.h"

namespace utils {
    class BoardPrinter {
        public:
            // Public member functions
            BoardPrinter(std::vector<U64*> bitboards);
            void fillBoard(std::vector<U64*> bitboards);
            void printBoard();

        private:
            // Private member variables
            std::vector<std::vector<char>> _board;

            // Private member functions
            bool isValidPiece(game::PieceType piece_type);
    };
}