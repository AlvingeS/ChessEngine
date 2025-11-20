namespace model {

inline const bitboard& Bitboards::get_bb_from_piece_type(Piece::Type piece_type) const 
{
    return *(bbs_[Piece::get_int_from_type(piece_type)]);
}

inline const bitboard& Bitboards::get_bb_from_index(int index) const 
{
    return *(bbs_[index]);
}

inline void Bitboards::clear_piece_type_bit(int index, Piece::Type piece_type)
{
    *(bbs_[Piece::get_int_from_type(piece_type)]) &= ~(1ULL << index);
}

inline void Bitboards::set_piece_type_bit(int index, Piece::Type piece_type) 
{
    *(bbs_[Piece::get_int_from_type(piece_type)]) |= (1ULL << index);
}

}