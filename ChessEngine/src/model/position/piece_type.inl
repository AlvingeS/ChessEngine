namespace model {

inline int Piece::getIntFromType(Piece::Type type)
{
    return static_cast<int>(type);
}

inline Piece::Type Piece::getTypeFromInt(int integer)
{
    return static_cast<Piece::Type>(integer);
}

} // namespace model