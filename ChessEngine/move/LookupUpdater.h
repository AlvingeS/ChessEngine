#pragma once

#include "ChessEngine/board/SquaresLookup.h"

namespace move {

class LookupUpdater {

public:
    // ** Constructor **
    LookupUpdater(board::SquaresLookup& squaresLookup);

    // ** Getters & Setters **
    
    // ** Functions **
    void makeCastleMove(const bool isWhite, const bool isKingSide);
    void unmakeCastleMove(const bool wasWhite, const bool wasKingSide);

    board::PieceType getPieceTypeAtIndex(const int index) 
    {
        return _squaresLookup.getPieceTypeAtIndex(index);
    }

    void removeCapturedPieceFromLookup(const int captureIndex) 
    {
        _squaresLookup.setPieceTypeAtIndex(captureIndex, board::PieceType::EMPTY);
    }

    board::PieceType getPromotionPieceType(const int promotionFlag, const bool isWhite);

    void setPieceTypeAtIndex(
        const int index, 
        const board::PieceType pieceType) 
    {
        _squaresLookup.setPieceTypeAtIndex(index, pieceType);
    }

private:

    // ** Data **
    board::SquaresLookup& _squaresLookup;
};

} // namespace move