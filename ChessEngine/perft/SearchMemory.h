#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/utils/BitBasics.h"

namespace perft {

constexpr unsigned char whiteKingSide = 0b0001;
constexpr unsigned char whiteQueenSide = 0b0010;
constexpr unsigned char whiteBoth = 0b0011;
constexpr unsigned char blackKingSide = 0b0100;
constexpr unsigned char blackQueenSide = 0b1000;
constexpr unsigned char blackBoth = 0b1100;

class SearchMemory {

public:
    SearchMemory(int maxDepth);

    unsigned char getCastlingRightsAtDepth(int depth)
    {
        return _castlingRights[depth];
    }

    board::PieceType getLastCapturedPieceAtDepth(int depth) 
    {
        return _lastCapturedPieces[depth];
    }

    void setLastCapturedPieceAtDepth(
        int currentDepth,
        board::PieceType pieceType) 
    {
        _lastCapturedPieces[currentDepth] = pieceType;
    }

    void setCastlingRights(
        int currentDepth, 
        move::Move move, 
        bool isWhite, 
        board::PieceType movedPieceType
    );

    void unsetCastlingRights(int currentDepth);

    U64 getEnPessantTargetAtDepth(int depth) 
    {
        return _enPessantTargets[depth];
    }

    void setEnPessantTargetAtDepth(int depth, U64 target) 
    {
        _enPessantTargets[depth] = target;
    }

    int getNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        return _noCapturedOrPawnMoveCounts[depth];
    }

    void incrementNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth]++;
    }

    void decrementNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth]--;
    }

    void resetNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth] = 0;
    }

private:
    int _maxDepth;
    std::vector<unsigned char> _castlingRights;
    std::vector<board::PieceType> _lastCapturedPieces;
    std::vector<U64> _enPessantTargets;
    std::vector<int> _noCapturedOrPawnMoveCounts;

    void removeCastlingRightsForRemainingDepths(
        int currentDepth, 
        unsigned char rightsToRemove
    );
    
    void restoreCastlingRightsForRemainingDepths(int currentDepth);
    void overrideCastlingRights(unsigned char rights);

};

}