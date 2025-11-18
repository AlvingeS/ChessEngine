namespace logic {

inline bool BitBasics::get_bit(bitmask a, int i) 
{
    return (a >> i) & 1;
}

inline int BitBasics::lsb_index(bitmask a) 
{
    return __builtin_ctzll(a);
}   

inline int BitBasics::msb_index(bitmask a) 
{
    return 63 - __builtin_clzll(a);
}

inline int BitBasics::pop_count(bitmask a) 
{
    return __builtin_popcountll(a);
}

} // namespace logic