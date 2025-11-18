namespace model {

inline int Piece::get_int_from_type(Piece::Type type)
{
    return static_cast<int>(type);
}

inline Piece::Type Piece::get_type_from_int(int integer)
{
    return static_cast<Piece::Type>(integer);
}

} // namespace model