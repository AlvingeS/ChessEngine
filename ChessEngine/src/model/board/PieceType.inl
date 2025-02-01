namespace model {

inline int pieceTypeToInt(PieceType piece_type) 
{
    return static_cast<int>(piece_type);
}

inline PieceType intToPieceType(int i) 
{
    return static_cast<PieceType>(i);
}

} // namespace model