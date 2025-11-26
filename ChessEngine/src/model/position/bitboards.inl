namespace model {

inline const bitboard& Bitboards::get_bb_from_piece_type(Piece::Type piece_type) const 
{
    return *(bbs_[Piece::get_int_from_type(piece_type)]);
}

inline const bitboard& Bitboards::get_bb_from_idx(int idx) const 
{
    return *(bbs_[idx]);
}

inline void Bitboards::clear_piece_type_bit(sq_idx sq, Piece::Type piece_type)
{
    *(bbs_[Piece::get_int_from_type(piece_type)]) &= ~(1ULL << sq);
}

inline void Bitboards::set_piece_type_bit(sq_idx sq, Piece::Type piece_type) 
{
    *(bbs_[Piece::get_int_from_type(piece_type)]) |= (1ULL << sq);
}

}