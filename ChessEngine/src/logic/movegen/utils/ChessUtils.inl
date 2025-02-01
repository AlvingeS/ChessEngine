namespace logic {

inline bitmask getFileMask(int file) 
{
    return FILE_MASK[file];
}

inline bitmask getRankMask(int rank) 
{
    return RANK_MASK[rank];
}

inline int rankFromBitIndex(int i) 
{
    return i / 8;
}

inline int fileFromBitIndex(int i) 
{
    return i % 8;
}

inline bitmask getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return whitePieces | blackPieces;
}

inline bitmask getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return ~getOccupiedSquaresBitmask(whitePieces, blackPieces);
}

inline bitmask getWhiteSquaresBitmask() 
{
    return 0xAA55AA55AA55AA55ULL;
}

inline bitmask getBlackSquaresBitmask() 
{
    return 0x55AA55AA55AA55AAULL;
}

inline int abs(int n) {
    return (n < 0) ? -n : n;
}

} // namespace logic