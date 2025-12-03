namespace logic {

inline void ZHasher::xor_piece_type_at(sq_idx sq, model::Piece::Type piece_type)
{
    z_hash_ ^= square_piece_type_keys[sq][static_cast<int>(piece_type)];
}

inline void ZHasher::xor_castle_right(CastleRightsType c_rights_type)
{
    z_hash_ ^= c_right_keys[static_cast<int>(c_rights_type)];
}

inline void ZHasher::xor_ep_file_at(int file)
{
    z_hash_ ^= ep_file_keys[file];
}

inline void ZHasher::toggle_side_to_move(bool is_w)
{
    z_hash_ ^= is_w_key;
}

inline void ZHasher::set_z_hash(uint64_t z_hash) 
{
    z_hash_ = z_hash;
}

inline uint64_t ZHasher::value() const
{
    return z_hash_;
}

} // namespace logic