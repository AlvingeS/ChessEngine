#pragma once
#include "PieceType.h"

namespace game {
    class Move {
    public:
        Move();
        Move(int sourceSquare, int destinationSquare, PieceType pieceType, bool isCapture, bool isPromotion, PieceType promotionPiece);
        
        // Getter methods for move attributes
        int getSourceSquare() const;
        int getDestinationSquare() const;
        PieceType getPieceType() const;
        bool isCapture() const;
        bool isPromotion() const;
        PieceType getPromotionPiece() const;

    private:
        int sourceSquare;         // Source square (e.g., A1)
        int destinationSquare;    // Destination square (e.g., A4)
        PieceType pieceType;      // Type of moving piece (rook, bishop, pawn, etc.)
        bool capture;             // Is this move a capture?
        bool promotion;           // Is this move a promotion?
        PieceType promotionPiece; // Promoted piece type (if it's a promotion)
    };
}
