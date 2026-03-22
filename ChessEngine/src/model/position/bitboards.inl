namespace model {

inline void Bitboards::set_bit_at(sq_t sq, PieceType piece_type) 
{
    //assert(piece_type != EMPTY);
    bbs_[piece_type] |= (1ULL << sq);
}

inline void Bitboards::clr_bit_at(sq_t sq, PieceType piece_type)
{
    //assert(piece_type != EMPTY);
    bbs_[piece_type] &= ~(1ULL << sq);
}

inline bool Bitboards::get_bit_at(sq_t sq, PieceType piece_type) const
{
    //assert(piece_type != EMPTY);
    return (bbs_[piece_type] >> sq) & 1;
}

inline const bitboard& Bitboards::get(PieceType piece_type) const 
{
    //assert(piece_type != EMPTY);
    return bbs_[piece_type];
}

inline bool Bitboards::get_w_bit(sq_t sq) const {
    //assert(sq >= 0 && sq <= 63);
    return (get_w() >> sq) & 1;
}


inline bitboard Bitboards::get_w() const 
{
    return (bbs_[0] | bbs_[1] | bbs_[2] | bbs_[3] | bbs_[4] | bbs_[5]);
}

inline bool Bitboards::get_b_bit(sq_t sq) const {
    //assert(sq >= 0 && sq <= 63);
    return (get_b() >> sq) & 1;
}

inline bitboard Bitboards::get_b() const 
{
    return (bbs_[6] | bbs_[7] | bbs_[8] | bbs_[9] | bbs_[10] | bbs_[11]);
}

inline bool Bitboards::get_occ_bit(sq_t sq) const {
    //assert(sq >= 0 && sq <= 63);
    return (get_occ() >> sq) & 1;
}

inline bitboard Bitboards::get_occ() const
{
    return (get_w() | get_b());
}

inline bool Bitboards::get_empty_bit(sq_t sq) const {
    //assert(sq >= 0 && sq <= 63);
    return (~get_occ() >> sq) & 1;
}

inline bitboard Bitboards::get_empty() const
{
    return ~get_occ();
}

} // namespace model