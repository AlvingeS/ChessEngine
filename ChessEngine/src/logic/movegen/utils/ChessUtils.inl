namespace logic {

inline bitmask ChessUtils::getFileMask(int file) 
{
    return FILE_MASK[file];
}

inline bitmask ChessUtils::getRankMask(int rank) 
{
    return RANK_MASK[rank];
}

inline int ChessUtils::rankFromBitIndex(int i) 
{
    return i / 8;
}

inline int ChessUtils::fileFromBitIndex(int i) 
{
    return i % 8;
}

inline bitmask ChessUtils::getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return whitePieces | blackPieces;
}

inline bitmask ChessUtils::getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return ~getOccupiedSquaresBitmask(whitePieces, blackPieces);
}

inline bitmask ChessUtils::getWhiteSquaresBitmask() 
{
    return 0xAA55AA55AA55AA55ULL;
}

inline bitmask ChessUtils::getBlackSquaresBitmask() 
{
    return 0x55AA55AA55AA55AAULL;
}

inline int ChessUtils::abs(int n) {
    return (n < 0) ? -n : n;
}

} // namespace logic