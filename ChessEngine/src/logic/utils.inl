namespace logic::utils
{

inline void set_bit(bitmask& mask, sq_t i) 
{
    mask |= (1ULL << i);    
}

inline bool get_bit(bitmask mask, sq_t i) 
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

inline int rank_from_sq(sq_t sq) 
{
    return sq / 8;
}

inline int file_from_sq(sq_t sq) 
{
    return sq % 8;
}

inline int determine_capture_sq(const model::Move& move, bool is_w)
{
    return move.is_ep_capture() ? is_w ? move.to() - 8 
                                       : move.to() + 8 
                                : move.to();
}

template<typename Func>
inline void controlled_for_each_bit(bitmask mask, Func f) {
    while (mask) {
        auto sq = lsb_idx(mask);

        if (f(sq) == LoopControl::Break)
            return;

        mask &= mask - 1;
    }
}

template<typename Func>
inline void for_each_bit(bitmask mask, Func f) {
    while (mask) {
        auto sq = lsb_idx(mask);
        f(sq);
        mask &= mask - 1;
    }
}

} // namespace logic::utils