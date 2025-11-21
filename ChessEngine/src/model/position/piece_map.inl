namespace model {

inline Piece::Type PieceMap::get_piece_type_at(sq_idx sq) const
{
    return piece_map_[sq];
}

inline void PieceMap::set_piece_type_at(sq_idx sq, Piece::Type piece_type)
{
    piece_map_[sq] = piece_type;
}

} // namespace model