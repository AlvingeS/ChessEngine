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

            void setLastCapturedPiece(PieceType pieceType) {
                _lastCapturedPiece = pieceType;
            }

            PieceType getLastCapturedPiece() {
                return _lastCapturedPiece;
            }

            bits::U64 getBitboard(PieceType pieceType) {
                return _bitboards[pieceTypeToInt(pieceType)];
            }

            std::vector<bits::U64>& getBitboards() {
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

            bool getHasCastled(bool isWhite) {
                return isWhite ? _whiteHasCastled : _blackHasCastled;
            }

            void setHasCastled(bool isWhite, bool hasCastled) {
                if (isWhite) {
                    _whiteHasCastled = hasCastled;
                } else {
                    _blackHasCastled = hasCastled;
                }
            }

            void setKingMoved(bool isWhite, bool hasMoved) {
                if (isWhite) {
                    _whiteKingMoved = hasMoved;
                } else {
                    _blackKingMoved = hasMoved;
                }
            }

            void setRookAMoved(bool isWhite, bool hasMoved) {
                if (isWhite) {
                    _whiteRookAMoved = hasMoved;
                } else {
                    _blackRookAMoved = hasMoved;
                }
            }

            void setRookHMoved(bool isWhite, bool hasMoved) {
                if (isWhite) {
                    _whiteRookHMoved = hasMoved;
                } else {
                    _blackRookHMoved = hasMoved;
                }
            }

            PieceType getPieceTypeAtIndex(int index) {
                return _squaresLookup[index];
            }
            
            bool kingSideCastlersHasMoved(bool isWhite);
            bool queenSideCastlersHasMoved(bool isWhite);
            
            bits::U64 getWhitePiecesBitmask();
            bits::U64 getBlackPiecesBitmask();
            bool _whiteRookAMoved;
            std::vector<PieceType> _squaresLookup;

        private:
            // Private member variables
            std::vector<bits::U64> _bitboards;
            bits::U64 _enPessantTarget;

            bool _whiteHasCastled;
            bool _blackHasCastled;
            bool _whiteKingMoved;
            bool _blackKingMoved;
            bool _whiteRookHMoved;
            bool _blackRookAMoved;
            bool _blackRookHMoved;
            PieceType _lastCapturedPiece;
            int _noCaptureOrPawnMoveCount;

            // Private member functions
            void initPieceBitboards();
            bits::U64 getBitboardFromIndex(int index);
            void fillSquaresLookup();
            PieceType getPromotionPieceType(int promotionFlag, bool isWhite);
            void makeCastleMove(bool isWhite, bool isKingSide);
            void unmakeCastleMove(bool wasWhite, bool wasKingSide);
            void setCastlingFlags(PieceType pieceType, int from);
            void unsetCastlingFlags(PieceType pieceType, int from);
    };
}