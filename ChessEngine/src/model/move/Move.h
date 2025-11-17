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
    
    Move();
    Move(int bitIndexFrom, int bitIndexTo, int flag);
    Move(const Move& move);

    int getMove() const;
    int getFlag() const;

    void operator=(const Move& move);
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    int getBitIndexFrom() const;
    int getBitIndexTo() const;
    bool isQuite() const;
    bool isDoublePawnPush() const;
    bool isKingCastle() const;
    bool isQueenCastle() const;
    bool isNormalCapture() const;
    bool isEpCapture() const;
    bool isKnightPromo() const;
    bool isBishopPromo() const;
    bool isRookPromo() const;
    bool isQueenPromo() const;
    bool isKnightPromoCapture() const;
    bool isBishopPromoCapture() const;
    bool isRookPromoCapture() const;
    bool isQueenPromoCapture() const;
    bool isAnyCapture() const;
    bool isAnyPromo() const;
    bool isAnyPromoCapture() const;
    bool isAnyCastle() const;

private:
    int _move;
};

} // namespace model

#include "model/move/Move.inl"

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