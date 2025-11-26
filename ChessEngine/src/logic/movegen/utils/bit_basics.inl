namespace logic {

inline bool bits::get_bit(bitmask mask, int i) 
{
    return (mask >> i) & 1;
}

inline int bits::lsb_idx(bitmask mask) 
{
    return __builtin_ctzll(mask);
}   

inline int bits::msb_idx(bitmask mask) 
{
    return 63 - __builtin_clzll(mask);
}

inline int bits::pop_count(bitmask mask) 
{
    return __builtin_popcountll(mask);
}

} // namespace logic