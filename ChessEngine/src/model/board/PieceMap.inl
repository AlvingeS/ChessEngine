namespace model {

inline PieceType PieceMap::getPieceTypeAtIndex(int index) const
{
    return _pieceMap[index];
}

inline void PieceMap::setPieceTypeAtIndex(int index, PieceType pieceType)
{
    _pieceMap[index] = pieceType;
}

} // namespace model