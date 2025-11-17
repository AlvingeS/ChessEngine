#pragma once

#include "model/Types.h"
#include "model/move/Move.h"
#include "model/position/PieceType.h"

namespace {
    constexpr unsigned char whiteKingSide = 0b0001;
    constexpr unsigned char whiteQueenSide = 0b0010;
    constexpr unsigned char whiteBoth = 0b0011;
    constexpr unsigned char blackKingSide = 0b0100;
    constexpr unsigned char blackQueenSide = 0b1000;
    constexpr unsigned char blackBoth = 0b1100;
} // namespace

namespace engine {

class SearchMemory {

public:
    SearchMemory(int maxDepth);

    unsigned char getCastlingRightsAtDepth(int depth) const
    {
        return _castlingRights[depth];
    }

    model::Piece::Type getLastCapturedPieceAtDepth(int depth) const
    {
        return _lastCapturedPieces[depth];
    }

    void setLastCapturedPieceAtDepth(
        int currentDepth,
        model::Piece::Type pieceType) 
    {
        _lastCapturedPieces[currentDepth] = pieceType;
    }

    void setCastlingRights(
        int currentDepth, 
        const model::Move& move, 
        bool isWhite, 
        model::Piece::Type movedPieceType
    );

    void unsetCastlingRights(int currentDepth);

    bitmask getEnPessantTargetAtDepth(int depth) const
    {
        return _enPessantTargets[depth];
    }

    void setEnPessantTargetAtDepth(int depth, bitmask target) 
    {
        _enPessantTargets[depth] = target;
    }

    int getNoCapturedOrPawnMoveCountAtDepth(int depth) const
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

    void handleNoCaptureCount(
        const model::Move& move, 
        int currentDepth, 
        model::Piece::Type  movedPieceType
    );

    void handleEnPessantMemory(
            const model::Move& move, 
            bool isWhite, 
            int currentDepth, 
            int toIndex
    );

private:
    int _maxDepth;
    std::vector<unsigned char> _castlingRights;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<bitmask> _enPessantTargets;
    std::vector<int> _noCapturedOrPawnMoveCounts;

    void removeCastlingRightsForRemainingDepths(
        int currentDepth, 
        unsigned char rightsToRemove
    );
    
    void restoreCastlingRightsForRemainingDepths(int currentDepth);
    void overrideCastlingRights(unsigned char rights);

};

} // namespace engine
