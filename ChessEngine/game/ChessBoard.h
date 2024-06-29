#pragma once

#include "ChessEngine/common.h"

#include <functional>

#include "ChessEngine/utils/BitBasics.h"

#include "PieceType.h"
#include "Move.h"

namespace game {
    class ChessBoard {
        public:

            // Public member functions
            ChessBoard();

            // FEN methods
            void setBoardFromFen(const std::string& fen);
            std::string getFenFromBoard();
            
            // FIXME: This doesn't do anything right now
            bool isDeadPosition();

            // Tears down bitboards to ensure safe state when ChessBoard is destroyed
            void resetBitboards() {
                _bitboards.clear();
            }

            int& getNoCaptureOrPawnMoveCount() {
                return _noCaptureOrPawnMoveCount;
            }

            void setEnPessantTargetAtIndex(int index) {
                _enPessantTarget = (1ULL << index);
            }

            PieceType getPieceTypeAtIndex(int index) {
                return _squaresLookup[index];
            }

            void setPieceTypeAtIndex(int index, PieceType pieceType) {
                _squaresLookup[index] = pieceType;
            }

            // Getters for game state
            U64 getBitboard(PieceType pieceType) {
                return *(_bitboards[pieceTypeToInt(pieceType)]);
            }

            U64& getBitboardFromIndex(int index) {
                return *(_bitboards[index]);
            }

            std::vector<U64*>& getBitboards() {
                return _bitboards;
            }

            std::vector<PieceType>& getSquaresLookup() {
                return _squaresLookup;
            }

            // Getters for masks
            U64& getWhitePiecesBitmask() {
                return _whitePiecesBitmask;
            }

            U64& getBlackPiecesBitmask() {
                return _blackPiecesBitmask;
            }

            U64& getOccupiedPiecesBitmask() {
                return _occupiedPiecesBitmask;
            }

            U64& getEmptySquaresBitmask() {
                return _emptySquaresBitmask;
            }

            // Getters for bitboards
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
            
            // Fill masks methods
            void fillWhitePiecesBitmask();
            void fillBlackPiecesBitmask();
            
            void fillOccupiedPiecesBitmask() {
                _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
            }

            void fillEmptySquaresBitmask() {
                _emptySquaresBitmask = ~_occupiedPiecesBitmask;
            }

        private:
            // Private member variables
            std::vector<U64*> _bitboards;
            std::vector<PieceType> _squaresLookup;

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

            U64 _enPessantTarget;

            // Masks
            U64 _whitePiecesBitmask;
            U64 _blackPiecesBitmask;
            U64 _occupiedPiecesBitmask;
            U64 _emptySquaresBitmask;

            int _noCaptureOrPawnMoveCount;

            // Private member functions
            void initPieceBitboards();
            void fillSquaresLookup();
    };
}