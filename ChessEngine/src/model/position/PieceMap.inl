namespace model {

inline Piece::Type PieceMap::getPieceTypeAtIndex(int index) const
{
    return _pieceMap[index];
}

inline void PieceMap::setPieceTypeAtIndex(int index, Piece::Type pieceType)
{
    _pieceMap[index] = pieceType;
}

} // namespace model