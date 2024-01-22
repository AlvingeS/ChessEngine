#pragma once
#include <unordered_map>
#include "PieceType.h"
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
            
            bits::U64 getWhitePiecesBitMask();
            bits::U64 getBlackPiecesBitMask();
            bits::U64 getWhiteSquaresBitMask();
            bits::U64 getBlackSquaresBitMask();

            private:
                std::unordered_map<PieceType, bits::U64> _bitboards;

                void initPieceBitboards();
    };
}