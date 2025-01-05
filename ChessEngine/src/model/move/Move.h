#pragma once

#include <functional>

namespace model {

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
    
    inline Move() {
        _move = 0;
    }

    inline Move(int bitIndexFrom, int bitIndexTo, int flag) {
        _move = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
    }

    inline Move(const Move& move) {
        _move = move._move;
    }

    inline int getMove() const {
        return _move;
    }

    inline int getFlag() const {
        return (_move >> 12) & 0xf;
    }

    inline void operator=(const Move& move) {
        _move = move._move;
    }

    inline bool operator==(const Move& move) const {
        return (_move & 0xffff) == (move._move & 0xffff);
    }

    inline bool operator!=(const Move& move) const {
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

    inline bool isKingCastle() const {
        return ((_move >> 12) & 0xf) == KING_CASTLE_FLAG;
    }

    inline bool isQueenCastle() const {
        return ((_move >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
    }

    inline bool isNormalCapture() const {
        return ((_move >> 12) & 0xf) == CAPTURE_FLAG;
    }

    inline bool isEpCapture() const {
        return ((_move >> 12) & 0xf) == EP_CAPTURE_FLAG;
    }

    inline bool isKnightPromo() const {
        return ((_move >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
    }

    inline bool isBishopPromo() const {
        return ((_move >> 12) & 0xf) == BISHOP_PROMO_FLAG;
    }

    inline bool isRookPromo() const {
        return ((_move >> 12) & 0xf) == ROOK_PROMO_FLAG;
    }

    inline bool isQueenPromo() const {
        return ((_move >> 12) & 0xf) == QUEEN_PROMO_FLAG;
    }

    inline bool isKnightPromoCapture() const {
        return ((_move >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
    }

    inline bool isBishopPromoCapture() const {
        return ((_move >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
    }

    inline bool isRookPromoCapture() const {
        return ((_move >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
    }

    inline bool isQueenPromoCapture() const {
        return ((_move >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
    }

    // Checks if the flag is any number between 1 and 6
    inline bool isAnyCapture() const {
        return getFlag() >= 1 && getFlag()  <= 6;
    }

    inline bool isAnyPromo() const {
        return getFlag() >= 3 && getFlag() <= 10;
    }

    inline bool isAnyPromoCapture() const {
        return getFlag() >= 3 && getFlag() <= 6;
    }

    inline bool isAnyCastle() const {
        return getFlag() >= 14;
    }

private:
    // Private member variables
    int _move;
};

} // namespace model

namespace std {
    template <>
    struct hash<model::Move> {
        size_t operator()(const model::Move& move) const {
            size_t h1 = std::hash<int>()(move.getBitIndexFrom());
            size_t h2 = std::hash<int>()(move.getBitIndexTo());
            size_t h3 = std::hash<int>()(move.getFlag());

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}