#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"
#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/move/MoveMaker.h"

#include "RayLogic.h"
#include "CommonLogic.h"
#include "CheckDetection.h"

namespace movegen {

constexpr U64 whiteKingSideCastleMask = 0x000000000000006ULL;
constexpr U64 whiteQueenSideCastleMask = 0x0000000000000070ULL;
constexpr U64 blackKingSideCastleMask = 0x0600000000000000ULL;
constexpr U64 blackQueenSideCastleMask = 0x7000000000000000ULL;

class CastlingGenerator {

public:
    // Constructor
    CastlingGenerator(
        board::BitBoards& bitboards,
        board::GameStateBitMasks& gameStateBitmasks,
        move::MoveMaker& moveMaker, int& moveIndex,
        CommonLogic* commonLogic,
        CheckDetection* checkDetection
    );
    
    // Public member functions
    void generate(
        bool isWhite, 
        std::vector<move::Move>& moveList, 
        unsigned char castlingRights
    );

private:
    // Private member functions
    board::BitBoards& _bitboards;
    board::GameStateBitMasks& _gameStateBitmasks;
    move::MoveMaker& _moveMaker;
    int& _moveIndex;
    CommonLogic* _commonLogic;
    CheckDetection* _checkDetection;

    // Memory
    U64 _whiteKingSideCastleBitmask;
    U64 _whiteQueenSideCastleBitmask;
    U64 _blackKingSideCastleBitmask;
    U64 _blackQueenSideCastleBitmask;

    // Private member functions
    void genSingleCastleMove(
        bool isWhite,
        bool isKingSide,
        std::vector<move::Move>& moveList
    );

    bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide);
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);
};

} // namespace movegen