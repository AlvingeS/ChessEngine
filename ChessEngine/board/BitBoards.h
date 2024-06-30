#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"

namespace board {
    class BitBoards {
        public:
            // ** Constructor **
            BitBoards();

            // ** Getters & Setters **
            
            U64& getBitboardFromPieceType(PieceType pieceType) {
                return *(_bitboards[pieceTypeToInt(pieceType)]);
            }

            U64& getBitboardFromIndex(int index) {
                return *(_bitboards[index]);
            }

            // TODO: This is a bad getter, it should be removed
            std::vector<U64*>& getBitboards() {
                return _bitboards;
            }

            U64& getWhitePawnsBitboard() {
                return _whitePawnsBitboard;
            }

            U64& getWhiteKnightsBitboard() {
                return _whiteKnightsBitboard;
            }

            U64& getWhiteBishopsBitboard() {
                return _whiteBishopsBitboard;
            }

            U64& getWhiteRooksBitboard() {
                return _whiteRooksBitboard;
            }

            U64& getWhiteQueensBitboard() {
                return _whiteQueensBitboard;
            }

            U64& getWhiteKingBitboard() {
                return _whiteKingBitboard;
            }

            U64& getBlackPawnsBitboard() {
                return _blackPawnsBitboard;
            }

            U64& getBlackKnightsBitboard() {
                return _blackKnightsBitboard;
            }

            U64& getBlackBishopsBitboard() {
                return _blackBishopsBitboard;
            }

            U64& getBlackRooksBitboard() {
                return _blackRooksBitboard;
            }

            U64& getBlackQueensBitboard() {
                return _blackQueensBitboard;
            }

            U64& getBlackKingBitboard() {
                return _blackKingBitboard;
            }
    
            // ** Functions **
            void resetBitboards() {
                for (int i = 0; i < 12; i++) {
                    *(_bitboards[i]) = 0;
                }
            }

        private:
            // ** Data **
            std::vector<U64*> _bitboards;

            // White bitboards
            U64 _whitePawnsBitboard;
            U64 _whiteKnightsBitboard;
            U64 _whiteBishopsBitboard;
            U64 _whiteRooksBitboard;
            U64 _whiteQueensBitboard;
            U64 _whiteKingBitboard;

            // Black bitboards
            U64 _blackPawnsBitboard;
            U64 _blackKnightsBitboard;
            U64 _blackBishopsBitboard;
            U64 _blackRooksBitboard;
            U64 _blackQueensBitboard;
            U64 _blackKingBitboard;
    };
}