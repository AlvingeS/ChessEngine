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

            bool getHasCastled(bool isWhite) {
                return isWhite ? _whiteHasCastled : _blackHasCastled;
            }

            void setHasCastled(bool isWhite) {
                if (isWhite) {
                    _whiteHasCastled = true;
                } else {
                    _blackHasCastled = true;
                }
            }

            void setKingMoved(bool isWhite) {
                if (isWhite) {
                    _whiteKingMoved = true;
                } else {
                    _blackKingMoved = true;
                }
            }

            void setRookAMoved(bool isWhite) {
                if (isWhite) {
                    _whiteRookAMoved = true;
                } else {
                    _blackRookAMoved = true;
                }
            }

            void setRookHMoved(bool isWhite) {
                if (isWhite) {
                    _whiteRookHMoved = true;
                } else {
                    _blackRookHMoved = true;
                }
            }
            
            bool kingSideCastlersHasMoved(bool isWhite);
            bool queenSideCastlersHasMoved(bool isWhite);
            
            bits::U64 getWhitePiecesBitmask();
            bits::U64 getBlackPiecesBitmask();

        private:
            // Private member variables
            std::unordered_map<PieceType, bits::U64> _bitboards;
            bits::U64 _enPessantTarget;

            bool _whiteHasCastled;
            bool _blackHasCastled;
            bool _whiteKingMoved;
            bool _blackKingMoved;
            bool _whiteRookAMoved;
            bool _whiteRookHMoved;
            bool _blackRookAMoved;
            bool _blackRookHMoved;

            // Private member functions
            void initPieceBitboards();
    };
}