namespace model {

inline Piece::Type PieceMap::get_piece_type_at_index(int index) const
{
    return piece_map_[index];
}

inline void PieceMap::set_piece_type_at_index(int index, Piece::Type piece_type)
{
    piece_map_[index] = piece_type;
}

} // namespace model