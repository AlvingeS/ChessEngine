namespace logic {

inline std::vector<int>& Containers::get_sliding_piece_quiet_moves_indices()
{
    return sliding_piece_quiet_moves_indices_;
}

inline std::vector<int>& Containers::get_leaping_piece_quiet_moves_indices()
{
    return leaping_piece_quiet_moves_indices_;
}

inline std::vector<int>& Containers::get_sliding_piece_capturable_moves_indices()
{
    return sliding_piece_capturable_moves_indices_;
}

inline std::vector<int>& Containers::get_leaping_piece_capturable_moves_indices()
{
    return leaping_piece_capturable_moves_indices_;
}

inline std::vector<int>& Containers::get_piece_position_indices()
{
    return piece_position_indices_;
}

} // namespace logic