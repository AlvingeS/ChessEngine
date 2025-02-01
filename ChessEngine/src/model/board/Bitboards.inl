namespace model {

inline const bitboard& Bitboards::getBitboardFromPieceType(PieceType pieceType) const 
{
    return *(_bitboards[pieceTypeToInt(pieceType)]);
}

inline const bitboard& Bitboards::getBitboardFromIndex(int index) const 
{
    return *(_bitboards[index]);
}

inline bool Bitboards::pieceTypeBitIsSet(int index, PieceType pieceType) const 
{
    return (*(_bitboards[pieceTypeToInt(pieceType)]) & (1ULL << index)) != 0;
}

inline void Bitboards::clearPieceTypeBit(int index, PieceType pieceType)
{
    *(_bitboards[pieceTypeToInt(pieceType)]) &= ~(1ULL << index);
}

inline void Bitboards::setPieceTypeBit(int index, PieceType pieceType) 
{
    *(_bitboards[pieceTypeToInt(pieceType)]) |= (1ULL << index);
}

}