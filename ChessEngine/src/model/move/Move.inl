namespace model {

inline Move::Move() {
    _move = 0;
}

inline Move::Move(int bitIndexFrom, int bitIndexTo, int flag) {
    _move = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
}

inline Move::Move(const Move& move) {
    _move = move._move;
}

inline int Move::getMove() const {
    return _move;
}

inline int Move::getFlag() const {
    return (_move >> 12) & 0xf;
}

inline void Move::operator=(const Move& move) {
    _move = move._move;
}

inline bool Move::operator==(const Move& move) const {
    return (_move & 0xffff) == (move._move & 0xffff);
}

inline bool Move::operator!=(const Move& move) const {
    return (_move & 0xffff) != (move._move & 0xffff);
}

inline int Move::getBitIndexFrom() const {
    return _move & 0x3f;
}

inline int Move::getBitIndexTo() const {
    return (_move >> 6) & 0x3f;
}

inline bool Move::isQuite() const {
    return ((_move >> 12) & 0xf) == QUITE_FLAG;
}

inline bool Move::isDoublePawnPush() const {
    return ((_move >> 12) & 0xf) == DOUBLE_PAWN_PUSH_FLAG;
}

inline bool Move::isKingCastle() const {
    return ((_move >> 12) & 0xf) == KING_CASTLE_FLAG;
}

inline bool Move::isQueenCastle() const {
    return ((_move >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
}

inline bool Move::isNormalCapture() const {
    return ((_move >> 12) & 0xf) == CAPTURE_FLAG;
}

inline bool Move::isEpCapture() const {
    return ((_move >> 12) & 0xf) == EP_CAPTURE_FLAG;
}

inline bool Move::isKnightPromo() const {
    return ((_move >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
}

inline bool Move::isBishopPromo() const {
    return ((_move >> 12) & 0xf) == BISHOP_PROMO_FLAG;
}

inline bool Move::isRookPromo() const {
    return ((_move >> 12) & 0xf) == ROOK_PROMO_FLAG;
}

inline bool Move::isQueenPromo() const {
    return ((_move >> 12) & 0xf) == QUEEN_PROMO_FLAG;
}

inline bool Move::isKnightPromoCapture() const {
    return ((_move >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
}

inline bool Move::isBishopPromoCapture() const {
    return ((_move >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
}

inline bool Move::isRookPromoCapture() const {
    return ((_move >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
}

inline bool Move::isQueenPromoCapture() const {
    return ((_move >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
}

// Checks if the flag is any number between 1 and 6
inline bool Move::isAnyCapture() const {
    return getFlag() >= 1 && getFlag()  <= 6;
}

inline bool Move::isAnyPromo() const {
    return getFlag() >= 3 && getFlag() <= 10;
}

inline bool Move::isAnyPromoCapture() const {
    return getFlag() >= 3 && getFlag() <= 6;
}

inline bool Move::isAnyCastle() const {
    return getFlag() >= 14;
}

} // namespace model