#pragma once
#include <unordered_map>
#include "ChessEngine/game/PieceType.h"
#include "ChessEngine/bits/BitBasics.h"
#include <string>

namespace game {
    class ChessBoard {
        public:
            ChessBoard();

            bits::U64 getBitboard(PieceType piece_type) {
                return _bitboards[piece_type];
            }

            std::unordered_map<PieceType, bits::U64> getBitboards() {
                return _bitboards;
            }

            void setBoardFromFen(const std::string& fen);

            private:
                std::unordered_map<PieceType, bits::U64> _bitboards;

                void initPieceBitboards();
                void initMacroBitboards();
                bits::U64 genWhiteBitboards();
                bits::U64 genBlackBitboards();
                bits::U64 genOccupiedBitboard();
                bits::U64 genEmptyBitboard();
    };
}