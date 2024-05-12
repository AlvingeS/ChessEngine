#pragma once
#include "ChessEngine/game/Move.h"
#include <vector>
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/bits/BitBasics.h"
#include "ChessEngine/bits/Castling.h"
#include "RayLogic.h"
#include "CommonLogic.h"
#include "CheckDetection.h"
#include <string>
#include "RookGenerator.h"
#include "BishopGenerator.h"
#include "KnightGenerator.h"
#include "QueenGenerator.h"
#include "KingGenerator.h"
#include "PawnGenerator.h"
#include "CastlingGenerator.h"

namespace movegen {
    class MoveGenerator {
        public:
            // Public member variables
            static constexpr int MAX_LEGAL_MOVES = 218;

            // Public member functions
            MoveGenerator(game::ChessBoard& board, game::MoveMaker& moveMaker);
            void resetMoves(std::vector<game::Move>& moveList);
            void resetMoveIndex();
            void setBoardFromFen(std::string fen);
            void genMoves(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights);
            void genRookMoves(bool isWhite, std::vector<game::Move>& moveList);
            void genBishopMoves(bool isWhite, std::vector<game::Move>& moveList);
            void genKnightMoves(bool isWite, std::vector<game::Move>& moveList);
            void genQueenMoves(bool isWhite, std::vector<game::Move>& moveList);
            void genKingMoves(bool isWhite, std::vector<game::Move>& moveList);
            void genPawnMoves(bool isWhite, std::vector<game::Move>& moveList);
            void genCastlingMoves(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights);
            bool isInCheck(bool isWhite);

            size_t getMoveIndex() {
                return _moveIndex;
            }

            game::ChessBoard& getBoard() {
                return _board;
            }

        private:
            // Private member variables
            game::ChessBoard& _board;
            CommonLogic _commonLogic;
            RayLogic _rayLogic;
            CheckDetection _checkDetection;
            RookGenerator _rookGenerator;
            BishopGenerator _bishopGenerator;
            KnightGenerator _knightGenerator;
            QueenGenerator _queenGenerator;
            KingGenerator _kingGenerator;
            PawnGenerator _pawnGenerator;
            CastlingGenerator _castlingGenerator;

            int _moveIndex = 0;
    };
}