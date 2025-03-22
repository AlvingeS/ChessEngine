namespace model {

inline const bitboard& Bitboards::getBitboardFromPieceType(Piece::Type pieceType) const 
{
    return *(_bitboards[Piece::getIntFromType(pieceType)]);
}

inline const bitboard& Bitboards::getBitboardFromIndex(int index) const 
{
    return *(_bitboards[index]);
}

inline bool Bitboards::pieceTypeBitIsSet(int index, Piece::Type pieceType) const 
{
    return (*(_bitboards[Piece::getIntFromType(pieceType)]) & (1ULL << index)) != 0;
}

inline void Bitboards::clearPieceTypeBit(int index, Piece::Type pieceType)
{
    *(_bitboards[Piece::getIntFromType(pieceType)]) &= ~(1ULL << index);
}

inline void Bitboards::setPieceTypeBit(int index, Piece::Type pieceType) 
{
    *(_bitboards[Piece::getIntFromType(pieceType)]) |= (1ULL << index);
}

}