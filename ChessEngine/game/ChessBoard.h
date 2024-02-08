#pragma once
#include <unordered_map>
#include "PieceType.h"
#include "ChessEngine/bits/BitBasics.h"
#include <string>
#include <functional>

namespace game {
    class ChessBoard {
        public:

            // Public member functions
            ChessBoard();

            bits::U64 getBitboard(PieceType pieceType) {
                return _bitboards[pieceType];
            }

            std::unordered_map<PieceType, bits::U64> getBitboards() {
                return _bitboards;
            }

            bits::U64 getEnPessantTarget() {
                return _enPessantTarget;
            }

            void setEnPessantTargetAtIndex(int index) {
                _enPessantTarget = (1ULL << index);
            }

            void setBoardFromFen(const std::string& fen);
            
            bits::U64 getWhitePiecesBitmask();
            bits::U64 getBlackPiecesBitmask();

        private:
            // Private member variables
            std::unordered_map<PieceType, bits::U64> _bitboards;
            bits::U64 _enPessantTarget;

            // Private member functions
            void initPieceBitboards();
    };
}