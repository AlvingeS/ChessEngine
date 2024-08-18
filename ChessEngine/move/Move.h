#pragma once

#include <functional>

#include "ChessEngine/board/PieceType.h"

namespace move {

class Move {

public:
    static const int QUITE_FLAG = 0;
    static const int CAPTURE_FLAG = 1;
    static const int EP_CAPTURE_FLAG = 2;
    static const int KNIGHT_PROMO_CAPTURE_FLAG = 3;
    static const int BISHOP_PROMO_CAPTURE_FLAG = 4;
    static const int ROOK_PROMO_CAPTURE_FLAG = 5;
    static const int QUEEN_PROMO_CAPTURE_FLAG = 6;
    static const int KNIGHT_PROMO_FLAG = 7;
    static const int BISHOP_PROMO_FLAG = 8;
    static const int ROOK_PROMO_FLAG = 9;
    static const int QUEEN_PROMO_FLAG = 10;
    static const int TBD_ONE_FLAG = 11;
    static const int TBD_TWO_FLAG = 12;
    static const int DOUBLE_PAWN_PUSH_FLAG = 13;
    static const int KING_CASTLE_FLAG = 14;
    static const int QUEEN_CASTLE_FLAG = 15;
    
    Move() {
        _move = 0;
    }

    // Public member functions            
    Move(int bitIndexFrom, int bitIndexTo, int flag) {
        _move = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
    }

    // Copy constructor
    Move(const Move& move) {
        _move = move._move;
    }

    int getMove() const {
        return _move;
    }

    int getFlag() const {
        return (_move >> 12) & 0xf;
    }

    void operator=(const Move& move) {
        _move = move._move;
    }

    bool operator==(const Move& move) const {
        return (_move & 0xffff) == (move._move & 0xffff);
    }

    bool operator!=(const Move& move) const {
        return (_move & 0xffff) != (move._move & 0xffff);
    }

    inline int getBitIndexFrom() const {
        return _move & 0x3f;
    }
    
    inline int getBitIndexTo() const {
        return (_move >> 6) & 0x3f;
    }
    
    inline bool isQuite() const {
        return ((_move >> 12) & 0xf) == QUITE_FLAG;
    }

    inline bool isDoublePawnPush() const {
        return ((_move >> 12) & 0xf) == DOUBLE_PAWN_PUSH_FLAG;
    }

    bool isKingCastle() const {
        return ((_move >> 12) & 0xf) == KING_CASTLE_FLAG;
    }

    bool isQueenCastle() const {
        return ((_move >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
    }

    bool isNormalCapture() const {
        return ((_move >> 12) & 0xf) == CAPTURE_FLAG;
    }

    bool isEpCapture() const {
        return ((_move >> 12) & 0xf) == EP_CAPTURE_FLAG;
    }

    bool isKnightPromo() const {
        return ((_move >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
    }

    bool isBishopPromo() const {
        return ((_move >> 12) & 0xf) == BISHOP_PROMO_FLAG;
    }

    bool isRookPromo() const {
        return ((_move >> 12) & 0xf) == ROOK_PROMO_FLAG;
    }

    bool isQueenPromo() const {
        return ((_move >> 12) & 0xf) == QUEEN_PROMO_FLAG;
    }

    bool isKnightPromoCapture() const {
        return ((_move >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
    }

    bool isBishopPromoCapture() const {
        return ((_move >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
    }

    bool isRookPromoCapture() const {
        return ((_move >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
    }

    bool isQueenPromoCapture() const {
        return ((_move >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
    }

    // Checks if the flag is any number between 1 and 6
    bool isAnyCapture() const {
        return getFlag() >= 1 && getFlag()  <= 6;
    }

    bool isAnyPromo() const {
        return getFlag() >= 3 && getFlag() <= 10;
    }

    bool isAnyPromoCapture() const {
        return getFlag() >= 3 && getFlag() <= 6;
    }

    bool isAnyCastle() const {
        return getFlag() >= 14;
    }

private:
    // Private member variables
    int _move;
};

} // namespace move

namespace std {
    template <>
    struct hash<move::Move> {
        size_t operator()(const move::Move& move) const {
            size_t h1 = std::hash<int>()(move.getBitIndexFrom());
            size_t h2 = std::hash<int>()(move.getBitIndexTo());
            size_t h3 = std::hash<int>()(move.getFlag());

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}