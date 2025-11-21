namespace model {

inline void ZHasher::hash_piece_type_at(sq_idx sq, Piece::Type piece_type)
{
    hash_ ^= random_board_piece_type_numbers_[sq][static_cast<int>(piece_type)];
}

inline void ZHasher::hash_castle_rights(int castle_rights)
{
    hash_ ^= random_castle_rights_numbers_[castle_rights];
}

inline void ZHasher::hash_ep_file(int file)
{
    hash_ ^= random_ep_file_numbers_[file];
}

inline void ZHasher::hash_is_w(bool is_w)
{
    if (!is_w) {
        hash_ ^= random_is_w_number_;
    }
}

} // namespace model