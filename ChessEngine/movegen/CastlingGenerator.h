#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/Move.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/MoveMaker.h"
#include "ChessEngine/bits/Castling.h"

#include "RayLogic.h"
#include "CommonLogic.h"
#include "CheckDetection.h"

namespace movegen {
    class CastlingGenerator {
        public:
            // Constructor
            CastlingGenerator(game::ChessBoard& board, game::MoveMaker& moveMaker, int& moveIndex, CommonLogic* commonLogic, CheckDetection* checkDetection);
            
            // Public member functions
            void generate(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights);
        private:

            // Private member functions
            game::ChessBoard& _board;
            game::MoveMaker& _moveMaker;
            int& _moveIndex;
            CommonLogic* _commonLogic;
            CheckDetection* _checkDetection;

            // Memory
            U64 _whiteKingSideCastleBitmask;
            U64 _whiteQueenSideCastleBitmask;
            U64 _blackKingSideCastleBitmask;
            U64 _blackQueenSideCastleBitmask;

            // Private member functions
            void genSingleCastleMove(bool isWhite, bool isKingSide, std::vector<game::Move>& moveList);
            bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide);
            void makeTemporaryKingMove(bool isWhite, bool isKingSide);
            void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);
    };
}