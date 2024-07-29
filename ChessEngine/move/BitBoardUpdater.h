#pragma once

#include "ChessEngine/board/BitBoards.h"

namespace move {

class BitBoardUpdater {

public:
    // ** Constructor **
    BitBoardUpdater(board::BitBoards& bitboards);

    // ** Getters & Setters **
    
    // ** Functions **
    void makeCastleMove(const bool isWhite, const bool isKingSide);
    void unmakeCastleMove(const bool wasWhite, const bool wasKingSide);
    
    void removeBitAtIndexForPieceType(
        const int captureIndex, 
        const board::PieceType capturedPieceType) 
    {
        _bitboards.getBitboardFromIndex(board::pieceTypeToInt(capturedPieceType)) &= ~(1ULL << captureIndex);
    }

    void setBitAtIndexForPieceType(
        const int toIndex, 
        const board::PieceType promotionPieceType) 
    {
        _bitboards.getBitboardFromIndex(board::pieceTypeToInt(promotionPieceType)) |= (1ULL << toIndex);
    }

    void makeTemporaryKingMove(const bool isWhite, const bool isKingSide);
    void unmakeTemporaryKingMove(const bool wasWhite, const bool wasKingSide);

private:
    // ** Data **
    board::BitBoards& _bitboards;
};

} // namespace move