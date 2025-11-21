namespace logic {

inline std::vector<int>& Containers::get_sliding_piece_quiet_moves_idxs()
{
    return sliding_piece_quiet_moves_idxs_;
}

inline std::vector<int>& Containers::get_leaping_piece_quiet_moves_idxs()
{
    return leaping_piece_quiet_moves_idxs_;
}

inline std::vector<int>& Containers::get_sliding_piece_capturable_moves_idxs()
{
    return sliding_piece_capturable_moves_idxs_;
}

inline std::vector<int>& Containers::get_leaping_piece_capturable_moves_idxs()
{
    return leaping_piece_capturable_moves_idxs_;
}

inline std::vector<int>& Containers::get_piece_position_idxs()
{
    return piece_position_idxs_;
}

} // namespace logic