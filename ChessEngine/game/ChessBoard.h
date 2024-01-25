#pragma once
#include <unordered_map>
#include "PieceType.h"
#include "ChessEngine/bits/BitBasics.h"
#include <string>

namespace game {
    class ChessBoard {
        public:

            // Public member functions
            ChessBoard();

            bits::U64 getBitboard(PieceType piece_type) {
                return _bitboards[piece_type];
            }

            std::unordered_map<PieceType, bits::U64> getBitboards() {
                return _bitboards;
            }

            void setBoardFromFen(const std::string& fen);
            
            bits::U64 getWhitePiecesBitmask();
            bits::U64 getBlackPiecesBitmask();

        private:
            // Private member variables
            std::unordered_map<PieceType, bits::U64> _bitboards;

            // Private member functions
            void initPieceBitboards();
    };
}