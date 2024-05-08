#pragma once
#include <unordered_map>
#include "PieceType.h"
#include "ChessEngine/bits/BitBasics.h"
#include <string>
#include <functional>
#include "Move.h"
#include <memory>
#include <vector>

namespace game {
    class ChessBoard {
        public:

            // Public member functions
            ChessBoard();

            void makeMove(Move move, bool isWhite);
            void unmakeMove(Move move, bool wasWhite);
            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool wasWhite, bool wasKingSide);
            bool isDeadPosition();

            // Tears down bitboards to ensure safe state when ChessBoard is destroyed
            void resetBitboards() {
                _bitboards.clear();
            }

            void setLastCapturedPiece(PieceType pieceType) {
                _lastCapturedPiece = pieceType;
            }

            PieceType getLastCapturedPiece() {
                return _lastCapturedPiece;
            }

            inline bits::U64 getBitboard(PieceType pieceType) {
                return *(_bitboards[pieceTypeToInt(pieceType)]);
            }

            std::vector<bits::U64*>& getBitboards() {
                return _bitboards;
            }

            std::vector<PieceType>& getSquaresLookup() {
                return _squaresLookup;
            }

            bits::U64 getEnPessantTarget() {
                return _enPessantTarget;
            }

            int getNoCaptureOrPawnMoveCount() {
                return _noCaptureOrPawnMoveCount;
            }

            void setEnPessantTargetAtIndex(int index) {
                _enPessantTarget = (1ULL << index);
            }

            void setBoardFromFen(const std::string& fen);
            std::string getFenFromBoard();

            PieceType getPieceTypeAtIndex(int index) {
                return _squaresLookup[index];
            }

            inline bits::U64 getWhitePiecesBitmask() {
                return _whitePiecesBitmask;
            }

            inline bits::U64 getBlackPiecesBitmask() {
                return _blackPiecesBitmask;
            }

            inline bits::U64 getOccupiedPiecesBitmask() {
                return _occupiedPiecesBitmask;
            }

            inline bits::U64 getEmptySquaresBitmask() {
                return _emptySquaresBitmask;
            }

            inline bits::U64 getWhitePawnsBitboard() {
                return _whitePawnsBitboard;
            }

            inline bits::U64 getWhiteKnightsBitboard() {
                return _whiteKnightsBitboard;
            }

            inline bits::U64 getWhiteBishopsBitboard() {
                return _whiteBishopsBitboard;
            }

            inline bits::U64 getWhiteRooksBitboard() {
                return _whiteRooksBitboard;
            }

            inline bits::U64 getWhiteQueensBitboard() {
                return _whiteQueensBitboard;
            }

            inline bits::U64 getWhiteKingBitboard() {
                return _whiteKingBitboard;
            }

            inline bits::U64 getBlackPawnsBitboard() {
                return _blackPawnsBitboard;
            }

            inline bits::U64 getBlackKnightsBitboard() {
                return _blackKnightsBitboard;
            }

            inline bits::U64 getBlackBishopsBitboard() {
                return _blackBishopsBitboard;
            }

            inline bits::U64 getBlackRooksBitboard() {
                return _blackRooksBitboard;
            }

            inline bits::U64 getBlackQueensBitboard() {
                return _blackQueensBitboard;
            }

            inline bits::U64 getBlackKingBitboard() {
                return _blackKingBitboard;
            }
            
            bool kingSideCastlersHasMoved(bool isWhite);
            bool queenSideCastlersHasMoved(bool isWhite);
            
            void fillWhitePiecesBitmask();
            void fillBlackPiecesBitmask();
            
            inline void fillOccupiedPiecesBitmask() {
                _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
            }
            inline void fillEmptySquaresBitmask() {
                _emptySquaresBitmask = ~_occupiedPiecesBitmask;
            }
            std::vector<PieceType> _squaresLookup;

        private:
            // Private member variables
            std::vector<bits::U64*> _bitboards;

            // White bitboards
            bits::U64 _whitePawnsBitboard;
            bits::U64 _whiteKnightsBitboard;
            bits::U64 _whiteBishopsBitboard;
            bits::U64 _whiteRooksBitboard;
            bits::U64 _whiteQueensBitboard;
            bits::U64 _whiteKingBitboard;

            // Black bitboards
            bits::U64 _blackPawnsBitboard;
            bits::U64 _blackKnightsBitboard;
            bits::U64 _blackBishopsBitboard;
            bits::U64 _blackRooksBitboard;
            bits::U64 _blackQueensBitboard;
            bits::U64 _blackKingBitboard;

            bits::U64 _enPessantTarget;

            bits::U64 _whitePiecesBitmask;
            bits::U64 _blackPiecesBitmask;
            bits::U64 _occupiedPiecesBitmask;
            bits::U64 _emptySquaresBitmask;

            PieceType _lastCapturedPiece;
            int _noCaptureOrPawnMoveCount;

            // Private member functions
            void initPieceBitboards();
            bits::U64 getBitboardFromIndex(int index);
            void fillSquaresLookup();
            PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
            bool castlingRightsNeedsUpdating(bool isWhite, Move move, PieceType movedPieceType);
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);

    };
}