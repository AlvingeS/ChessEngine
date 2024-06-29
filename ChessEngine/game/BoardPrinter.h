#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"

namespace game {
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
            bool isValidPiece(PieceType piece_type);
    };
}