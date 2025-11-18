namespace model {

inline Move::Move() {
    move_ = 0;
}

inline Move::Move(int bitIndexFrom, int bitIndexTo, int flag) {
    move_ = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
}

inline Move::Move(const Move& move) {
    move_ = move.move_;
}

inline int Move::get_move() const {
    return move_;
}

inline int Move::get_flag() const {
    return (move_ >> 12) & 0xf;
}

inline void Move::operator=(const Move& move) {
    move_ = move.move_;
}

inline bool Move::operator==(const Move& move) const {
    return (move_ & 0xffff) == (move.move_ & 0xffff);
}

inline bool Move::operator!=(const Move& move) const {
    return (move_ & 0xffff) != (move.move_ & 0xffff);
}

inline int Move::get_bit_index_from() const {
    return move_ & 0x3f;
}

inline int Move::get_bit_index_to() const {
    return (move_ >> 6) & 0x3f;
}

inline bool Move::is_quite() const {
    return ((move_ >> 12) & 0xf) == QUITE_FLAG;
}

inline bool Move::is_double_pawn_push() const {
    return ((move_ >> 12) & 0xf) == DOUBLE_PAWN_PUSH_FLAG;
}

inline bool Move::is_king_castle() const {
    return ((move_ >> 12) & 0xf) == KING_CASTLE_FLAG;
}

inline bool Move::is_queen_castle() const {
    return ((move_ >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
}

inline bool Move::is_normal_capture() const {
    return ((move_ >> 12) & 0xf) == CAPTURE_FLAG;
}

inline bool Move::is_ep_capture() const {
    return ((move_ >> 12) & 0xf) == EP_CAPTURE_FLAG;
}

inline bool Move::is_knight_promo() const {
    return ((move_ >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
}

inline bool Move::is_bishop_promo() const {
    return ((move_ >> 12) & 0xf) == BISHOP_PROMO_FLAG;
}

inline bool Move::is_rook_promo() const {
    return ((move_ >> 12) & 0xf) == ROOK_PROMO_FLAG;
}

inline bool Move::is_queen_promo() const {
    return ((move_ >> 12) & 0xf) == QUEEN_PROMO_FLAG;
}

inline bool Move::is_knight_promo_capture() const {
    return ((move_ >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
}

inline bool Move::is_bishop_promo_capture() const {
    return ((move_ >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
}

inline bool Move::is_rook_promo_capture() const {
    return ((move_ >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
}

inline bool Move::is_queen_promo_capture() const {
    return ((move_ >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
}

// Checks if the flag is any number between 1 and 6
inline bool Move::is_any_capture() const {
    return get_flag() >= 1 && get_flag()  <= 6;
}

inline bool Move::is_any_promo() const {
    return get_flag() >= 3 && get_flag() <= 10;
}

inline bool Move::is_any_promo_capture() const {
    return get_flag() >= 3 && get_flag() <= 6;
}

inline bool Move::is_any_castle() const {
    return get_flag() >= 14;
}

} // namespace model