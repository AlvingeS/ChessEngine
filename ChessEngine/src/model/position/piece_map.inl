namespace model {

inline Piece::Type PieceMap::get_piece_type_at_idx(int idx) const
{
    return piece_map_[idx];
}

inline void PieceMap::set_piece_type_at_idx(int idx, Piece::Type piece_type)
{
    piece_map_[idx] = piece_type;
}

} // namespace model