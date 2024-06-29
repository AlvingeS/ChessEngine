#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/PieceType.h"
#include "ChessEngine/game/BitBoards.h"

namespace utils {
    class BoardPrinter {
        public:
            // Public member functions
            BoardPrinter(game::BitBoards& bitboards);
            void fillBoard(game::BitBoards& bitboards);
            void printBoard();

        private:
            // Private member variables
            std::vector<std::vector<char>> _board;

            // Private member functions
            bool isValidPiece(game::PieceType piece_type);
    };
}