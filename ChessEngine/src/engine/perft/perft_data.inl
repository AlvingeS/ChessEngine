namespace engine {

inline int PerftData::get_node_count_at(int depth) const { return node_count_[depth]; }
inline int PerftData::get_capture_count_at(int depth) const { return capture_count_[depth]; }
inline int PerftData::get_ep_capture_count_at(int depth) const { return ep_capture_count_[depth]; }
inline int PerftData::get_castle_rights_at(int depth) const { return casle_count_[depth]; }
inline int PerftData::get_promotion_count_at(int depth) const { return promo_count_[depth]; }
inline int PerftData::get_check_count_at(int depth) const { return check_count_[depth]; }
inline int PerftData::get_checkmate_count_at(int depth) const { return checkmate_count_[depth]; }

inline void PerftData::increase_node_count_at(int depth) { node_count_[depth]++; }
inline void PerftData::increase_capture_count_at(int depth) { capture_count_[depth]++; }
inline void PerftData::increase_ep_capture_count_at(int depth) { ep_capture_count_[depth]++; }
inline void PerftData::increase_castle_count_at(int depth) { casle_count_[depth]++; }
inline void PerftData::increase_promo_count_at(int depth) { promo_count_[depth]++; }
inline void PerftData::increase_check_count_at(int depth) { check_count_[depth]++; }
inline void PerftData::increase_checkmate_count_at(int depth) { checkmate_count_[depth]++; }

inline void PerftData::set_first_move_at(int first_move_idx, model::Move currentMove) 
{
    first_moves_[first_move_idx] = currentMove;
}

inline void PerftData::increase_node_count_per_first_move_at(int first_move_idx) 
{
    node_count_per_first_move_[first_move_idx]++;
}

} // namespace engine