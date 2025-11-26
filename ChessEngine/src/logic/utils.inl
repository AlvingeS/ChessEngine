namespace logic::utils
{

inline bool get_bit(bitmask mask, int i) 
{
    return (mask >> i) & 1;
}

inline int lsb_idx(bitmask mask) 
{
    return __builtin_ctzll(mask);
}   

inline int msb_idx(bitmask mask) 
{
    return 63 - __builtin_clzll(mask);
}

inline int pop_count(bitmask mask) 
{
    return __builtin_popcountll(mask);
}

inline bitmask get_file_mask(int file) 
{
    return logic::masks::FILE_MASK[file];
}

inline bitmask get_rank_mask(int rank) 
{
    return logic::masks::RANK_MASK[rank];
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

inline int determine_capture_sq(const model::Move& move, bool is_w)
{
    return move.is_ep_capture() ? is_w ? move.get_to_sq() - 8 
                                       : move.get_to_sq() + 8 
                                : move.get_to_sq();
}

} // namespace logic::utils