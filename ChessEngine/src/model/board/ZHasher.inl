namespace model {

inline void ZHasher::hashSquarePieceType(int squareIndex, PieceType pieceType)
{
    _hash ^= _randBoardPieceTypeNums[squareIndex][static_cast<int>(pieceType)];
}

inline void ZHasher::hashCastleRights(int castleRights)
{
    _hash ^= _randCastleRightsNums[castleRights];
}

inline void ZHasher::hashEnPassantFile(int file)
{
    _hash ^= _randEnPassantFileNums[file];
}

inline void ZHasher::hashIsWhite(bool isWhite)
{
    _hash ^= _randIsWhiteNum;
}

} // namespace model