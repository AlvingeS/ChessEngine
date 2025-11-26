namespace model {

inline void OccupancyMasks::set_w_pieces_bit(sq_idx sq) 
{
    w_pieces_mask_ |= (1ULL << sq);
}

inline void OccupancyMasks::clear_w_pieces_bit(sq_idx sq) 
{
    w_pieces_mask_ &= ~(1ULL << sq);
}

inline const bitmask& OccupancyMasks::get_w_pieces_mask() const 
{
    return w_pieces_mask_;
}

inline void OccupancyMasks::set_b_pieces_bit(sq_idx sq) 
{
    b_pieces_mask_ |= (1ULL << sq);
}

inline void OccupancyMasks::clear_b_pieces_bit(sq_idx sq) 
{
    b_pieces_mask_ &= ~(1ULL << sq);
}

inline const bitmask& OccupancyMasks::get_b_pieces_mask() const 
{
    return b_pieces_mask_;
}

inline const bitmask& OccupancyMasks::get_occupied_squares_mask() const 
{
    return occupied_squares_mask_;
}

inline const bitmask& OccupancyMasks::get_free_squares_mask() const 
{
    return free_squares_mask_;
}

inline void OccupancyMasks::update_occupied_squares_mask() 
{
    occupied_squares_mask_ = w_pieces_mask_ | b_pieces_mask_;
}

inline void OccupancyMasks::update_free_squares_mask() 
{
    free_squares_mask_ = ~occupied_squares_mask_;
}

} // namespace model