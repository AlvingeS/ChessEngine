#pragma once
#include "Move.h"
#include <vector>
#include "ChessBoard.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/BitBasics.h"
#include "ChessEngine/bits/Castling.h"

namespace game {
    class MoveGenerator {
        public:
            // Public member variables
            static constexpr int MAX_LEGAL_MOVES = 218;

            // Public member functions
            MoveGenerator(ChessBoard& board);
            bool isInCheck(bool isWhite);
            bool isDeadPosition();
            std::vector<Move>& genMoves(bool isWhite);
            void resetMoves();
            void resetMoveIndex();
            int getNumLegalMoves();
            void setBoardFromFen(std::string fen);
            void genRookMoves(bool isWhite);
            void genBishopMoves(bool isWhite);
            void genKnightMoves(bool isWite);
            void genQueenMoves(bool isWhite);
            void genKingMoves(bool isWhite);
            void genPawnMoves(bool isWhite);
            void genCastlingMoves(bool isWhite);
            void updateGameStateBitmasks();

            std::vector<Move>& getMoves() {
                return _moves;
            }

            size_t getMoveIndex() {
                return _moveIndex;
            }

            ChessBoard& getBoard() {
                return _board;
            }

        private:
            // Private member variables
            std::vector<Move> _moves;

            ChessBoard& _board;
            size_t _moveIndex = 0;

            std::vector<bits::StraightRays> _straightRayBitmasks;
            std::vector<bits::DiagonalRays> _diagonalRayBitmasks;
            std::vector<bits::U64> _knightBitmasks;
            std::vector<bits::U64> _kingBitmasks;
            std::vector<bits::U64> _whitePawnStraightMoveBitmasks;
            std::vector<bits::U64> _blackPawnStraightMoveBitmasks;
            std::vector<bits::U64> _whitePawnCaptureMoveBitmasks;
            std::vector<bits::U64> _blackPawnCaptureMoveBitmasks;

            bits::U64 _occupiedBitmask;
            bits::U64 _emptySquaresBitmask;
            bits::U64 _whitePiecesBitmask;
            bits::U64 _blackPiecesBitmask;
            bits::U64 _whiteSquaresBitmask;            
            bits::U64 _blackSquaresBitmask;
            bits::U64 _whiteQueenSideCastleBitmask;
            bits::U64 _whiteKingSideCastleBitmask;
            bits::U64 _blackQueenSideCastleBitmask;
            bits::U64 _blackKingSideCastleBitmask;

            std::vector<int> _freeRayIndices;
            std::vector<int> _rookIndices;
            std::vector<int> _bishopIndices;
            std::vector<int> _queenIndices;
            std::vector<int> _knightIndices;
            std::vector<int> _kingIndices;
            std::vector<int> _pawnIndices;
            std::vector<int> _freeMovesIndices;
            std::vector<int> _capturableMovesIndices;

            // Member functions
            void addMove(int bitIndexFrom, int bitIndexTo, int flag);
            void addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom);
            void addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom);
            void addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, bool startFromBlocker, int rookRank, int rookFile, int bitIndexFrom);
            void addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, int bishopRank, int bishopFile, int bitIndexFrom);
            void getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, int pieceRank, int pieceFile);
            void getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, int pieceRank, int pieceFile);
            bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide);

            // Check detection
            bool checkStraightRay(bits::U64& straightRay, bool firstBlockerOnLSB, bits::U64& opponentRooksAndQueens);
            bool checkDiagonalRay(bits::U64& diagonalRay, bool firstBlockerOnLSB, bits::U64& opponentBishopsAndQueens);
    };
}