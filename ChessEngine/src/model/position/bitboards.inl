namespace model {

inline const bitboard& Bitboards::get_bitboard_from_piece_type(Piece::Type piece_type) const 
{
    return *(_bitboards[Piece::get_int_from_type(piece_type)]);
}

inline const bitboard& Bitboards::get_bitboard_from_index(int index) const 
{
    return *(_bitboards[index]);
}

inline bool Bitboards::piece_type_bit_is_set(int index, Piece::Type piece_type) const 
{
    return (*(_bitboards[Piece::get_int_from_type(piece_type)]) & (1ULL << index)) != 0;
}

inline void Bitboards::clear_piece_type_bit(int index, Piece::Type piece_type)
{
    *(_bitboards[Piece::get_int_from_type(piece_type)]) &= ~(1ULL << index);
}

inline void Bitboards::set_piece_type_bit(int index, Piece::Type piece_type) 
{
    *(_bitboards[Piece::get_int_from_type(piece_type)]) |= (1ULL << index);
}

}