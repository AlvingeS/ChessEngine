namespace model {

inline void OccupancyMasks::set_w_pieces_bit(int index) 
{
    w_pieces_mask_ |= (1ULL << index);
}

inline void OccupancyMasks::clear_w_pieces_bit(int index) 
{
    w_pieces_mask_ &= ~(1ULL << index);
}

inline const bitmask& OccupancyMasks::get_w_pieces_mask() const 
{
    return w_pieces_mask_;
}

inline void OccupancyMasks::set_b_pieces_bit(int index) 
{
    b_pieces_mask_ |= (1ULL << index);
}

inline void OccupancyMasks::clear_b_pieces_bit(int index) 
{
    b_pieces_mask_ &= ~(1ULL << index);
}

inline const bitmask& OccupancyMasks::get_b_pieces_mask() const 
{
    return b_pieces_mask_;
}

inline const bitmask& OccupancyMasks::get_occupied_squares_mask() const 
{
    return occupied_sqrs_mask_;
}

inline const bitmask& OccupancyMasks::get_free_squares_mask() const 
{
    return free_sqrs_mask_;
}

inline void OccupancyMasks::update_occupied_sqrs_mask() 
{
    occupied_sqrs_mask_ = w_pieces_mask_ | b_pieces_mask_;
}

inline void OccupancyMasks::update_free_sqrs_mask() 
{
    free_sqrs_mask_ = ~occupied_sqrs_mask_;
}

} // namespace model