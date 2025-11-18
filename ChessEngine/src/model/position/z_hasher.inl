namespace model {

inline void ZHasher::hash_square_piece_type(int square_index, Piece::Type piece_type)
{
    hash_ ^= random_board_piece_type_numbers_[square_index][static_cast<int>(piece_type)];
}

inline void ZHasher::hash_castle_rights(int castle_rights)
{
    hash_ ^= random_castle_rights_numbers_[castle_rights];
}

inline void ZHasher::hash_en_pessant_file(int file)
{
    hash_ ^= random_en_pessant_file_numbers_[file];
}

inline void ZHasher::hash_is_white(bool is_white)
{
    if (!is_white) {
        hash_ ^= random_is_white_number_;
    }
}

} // namespace model