namespace model {

inline bool StateBitmasks::w_pieces_bit_is_set(int index) const 
{
    return (w_pieces_bitmask_ & (1ULL << index)) != 0;
}

inline bool StateBitmasks::b_pieces_bit_is_set(int index) const 
{
    return (b_pieces_bitmask_ & (1ULL << index)) != 0;
}

inline void StateBitmasks::set_w_pieces_bit(int index) 
{
    w_pieces_bitmask_ |= (1ULL << index);
}

inline void StateBitmasks::clear_w_pieces_bit(int index) 
{
    w_pieces_bitmask_ &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::get_w_pieces_bitmask() const 
{
    return w_pieces_bitmask_;
}

inline void StateBitmasks::set_b_pieces_bit(int index) 
{
    b_pieces_bitmask_ |= (1ULL << index);
}

inline void StateBitmasks::clear_b_pieces_bit(int index) 
{
    b_pieces_bitmask_ &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::get_b_pieces_bitmask() const 
{
    return b_pieces_bitmask_;
}

inline const bitmask& StateBitmasks::get_occupied_pieces_bitmask() const 
{
    return occupied_pieces_bitmask_;
}

inline const bitmask& StateBitmasks::get_empty_squares_bitmask() const 
{
    return empty_squares_bitmask_;
}

inline void StateBitmasks::update_occupied_pieces_bitmask() 
{
    occupied_pieces_bitmask_ = w_pieces_bitmask_ | b_pieces_bitmask_;
}

inline void StateBitmasks::update_empty_squares_bitmask() 
{
    empty_squares_bitmask_ = ~occupied_pieces_bitmask_;
}

} // namespace model