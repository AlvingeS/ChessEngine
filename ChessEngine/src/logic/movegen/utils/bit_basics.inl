namespace logic {

inline bool BitBasics::get_bit(bitmask mask, int i) 
{
    return (mask >> i) & 1;
}

inline int BitBasics::lsb_idx(bitmask mask) 
{
    return __builtin_ctzll(mask);
}   

inline int BitBasics::msb_idx(bitmask mask) 
{
    return 63 - __builtin_clzll(mask);
}

inline int BitBasics::pop_count(bitmask mask) 
{
    return __builtin_popcountll(mask);
}

} // namespace logic