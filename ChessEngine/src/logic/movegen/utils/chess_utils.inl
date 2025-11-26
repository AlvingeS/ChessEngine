namespace {

    // TODO: Move to model/masks
    constexpr bitmask FILE_MASK[8] = {
        0x0101010101010101ULL,
        0x0202020202020202ULL,
        0x0404040404040404ULL,
        0x0808080808080808ULL,
        0x1010101010101010ULL,
        0x2020202020202020ULL,
        0x4040404040404040ULL,
        0x8080808080808080ULL
    };
    
    constexpr bitmask RANK_MASK[8] = {
        0x00000000000000FFULL,
        0x000000000000FF00ULL,
        0x0000000000FF0000ULL,
        0x00000000FF000000ULL,
        0x000000FF00000000ULL,
        0x0000FF0000000000ULL,
        0x00FF000000000000ULL,
        0xFF00000000000000ULL
    };

} // namespace 

namespace logic::chess_utils {

inline bitmask get_file_mask(int file) 
{
    return FILE_MASK[file];
}

inline bitmask get_rank_mask(int rank) 
{
    return RANK_MASK[rank];
}

inline int rank_from_sq(sq_idx sq) 
{
    return sq / 8;
}

inline int file_from_sq(sq_idx sq) 
{
    return sq % 8;
}

inline int abs(int n) {
    return (n < 0) ? -n : n;
}

} // namespace logic::chess_utils