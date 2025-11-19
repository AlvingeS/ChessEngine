namespace engine {

inline int perftData::getNodeCountAt(int depth) const { return node_count_[depth]; }
inline int perftData::getCaptureCountAt(int depth) const { return capture_count_[depth]; }
inline int perftData::getEpCaptureCountAt(int depth) const { return ep_capture_count_[depth]; }
inline int perftData::getCastlingCountAt(int depth) const { return casle_count_[depth]; }
inline int perftData::getPromotionCountAt(int depth) const { return promo_count_[depth]; }
inline int perftData::getCheckCountAt(int depth) const { return check_count_[depth]; }
inline int perftData::getCheckmateCountAt(int depth) const { return checkmate_count_[depth]; }

inline void perftData::increaseNodeCountAt(int depth) { node_count_[depth]++; }
inline void perftData::increaseCaptureCountAt(int depth) { capture_count_[depth]++; }
inline void perftData::increaseEpCaptureCountAt(int depth) { ep_capture_count_[depth]++; }
inline void perftData::increaseCastlingCountAt(int depth) { casle_count_[depth]++; }
inline void perftData::increasePromotionCountAt(int depth) { promo_count_[depth]++; }
inline void perftData::increaseCheckCountAt(int depth) { check_count_[depth]++; }
inline void perftData::increaseCheckmateCountAt(int depth) { checkmate_count_[depth]++; }

inline void perftData::setFirstMoveAt(int firstMoveIndex, model::Move currentMove) 
{
    first_moves_[firstMoveIndex] = currentMove;
}

inline void perftData::increaseNodeCountPerFirstMoveAt(int firstMoveIndex) 
{
    node_count_per_first_move_[firstMoveIndex]++;
}

} // namespace engine