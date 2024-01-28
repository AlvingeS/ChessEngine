#pragma once
#include "Move.h"
#include <vector>
#include "ChessBoard.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/BitBasics.h"

namespace game {
    class MoveGenerator {
        public:
            // Public member variables
            static constexpr int MAX_LEGAL_MOVES = 218;

            // Public member functions
            MoveGenerator();
            std::vector<Move>& genMoves(bool isWhite);
            int getNumLegalMoves();
            void setBoardFromFen(std::string fen);
            void genRookMoves(bool isWhite);
            void genBishopMoves(bool isWhite);
            void genKnightMoves(bool isWite);
            void genQueenMoves(bool isWhite);
            void genKingMoves(bool isWhite);

            std::vector<Move>& getMoves() {
                return _moves;
            }

        private:
            // Private member variables
            std::vector<Move> _moves; 
            ChessBoard _board;
            std::vector<bits::StraightRays> _straightRayBitmasks;
            std::vector<bits::DiagonalRays> _diagonalRayBitmasks;
            std::vector<bits::U64> _knightBitmasks;
            std::vector<bits::U64> _kingBitmasks;
            bits::U64 _occupiedBitmask;
            bits::U64 _emptySquaresBitmask;
            bits::U64 _whitePiecesBitmask;
            bits::U64 _blackPiecesBitmask;
            bits::U64 _whiteSquaresBitmask;            
            bits::U64 _blackSquaresBitmask;

            // Member functions
            void updateGameStateBitmasks();
            void addMove(int bitIndexFrom, int bitIndexTo, PieceType pieceType);
            void addMovesFromFreeRay(bits::U64 freeRay, int bitIndexFrom, PieceType pieceType);
            void addMoveIfBlockerIsEnemy(int blockerIndex, bool isWhite, int bitIndexFrom, PieceType pieceType);
            void addMovesBetweenBlockerAndPieceOnStraightRay(int blockerIndex, bool alongFile, bool startFromBlocker, int rookRank, int rookFile, PieceType pieceType, int bitIndexFrom);
            void addMovesBetweenBlockerAndPieceOnDiagonalRay(int blockerIndex, bool startFromBlocker, int bishopRank, int bishopFile, PieceType pieceType, int bitIndexFrom);
            void getMovesFromStraightRay(bits::U64 ray, bool blockerOnLSB, bool alongFile, bool isWhite, int pieceIndex, PieceType pieceType, int pieceRank, int pieceFile);
            void getMovesFromDiagonalRay(bits::U64 ray, bool blockerOnLSB, bool isWhite, int pieceIndex, PieceType pieceType, int pieceRank, int pieceFile);
    };
}