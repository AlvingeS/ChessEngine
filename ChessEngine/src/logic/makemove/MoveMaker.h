#pragma once

#include "logic/makemove/MoveUtils.h"

namespace model {
    class Board;
    class Move;
    class Bitboards;
    class StateBitmasks;
    class PieceMap;
    class ZHasher;
}

namespace logic {

class MoveMaker {

public:
    MoveMaker(model::Board& board);
    
    MoveResult makeMove(const model::Move& move, bool isWhite);
    
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

private:
    void makeCastleMove(bool isWhite, bool isKingSide);
    
    model::Piece::Type removeMovedPieceFromBoard(
        bool isWhite, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool isWhite, 
        int captureIndex, 
        model::Piece::Type capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
        model::Piece::Type movedPieceType
    );

    model::Bitboards& _bitboards;
    model::StateBitmasks& _stateBitmasks;
    model::PieceMap& _pieceMap;
    model::ZHasher& _zHasher;
};

} // namespace logic