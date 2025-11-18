namespace logic {

inline bool BitBasics::getBit(bitmask a, int i) 
{
    return (a >> i) & 1;
}

inline int BitBasics::indexOfLSB(bitmask a) 
{
    return __builtin_ctzll(a);
}   

inline int BitBasics::indexOfMSB(bitmask a) 
{
    return 63 - __builtin_clzll(a);
}

inline int BitBasics::popCount(bitmask a) 
{
    return __builtin_popcountll(a);
}

} // namespace logic