namespace engine {

inline int perftData::getNodeCountAt(int depth) const { return _nodeCount[depth]; }
inline int perftData::getCaptureCountAt(int depth) const { return _captureCount[depth]; }
inline int perftData::getEpCaptureCountAt(int depth) const { return _epCaptureCount[depth]; }
inline int perftData::getCastlingCountAt(int depth) const { return _castlingCount[depth]; }
inline int perftData::getPromotionCountAt(int depth) const { return _promotionCount[depth]; }
inline int perftData::getCheckCountAt(int depth) const { return _checkCount[depth]; }
inline int perftData::getCheckmateCountAt(int depth) const { return _checkmateCount[depth]; }

inline void perftData::increaseNodeCountAt(int depth) { _nodeCount[depth]++; }
inline void perftData::increaseCaptureCountAt(int depth) { _captureCount[depth]++; }
inline void perftData::increaseEpCaptureCountAt(int depth) { _epCaptureCount[depth]++; }
inline void perftData::increaseCastlingCountAt(int depth) { _castlingCount[depth]++; }
inline void perftData::increasePromotionCountAt(int depth) { _promotionCount[depth]++; }
inline void perftData::increaseCheckCountAt(int depth) { _checkCount[depth]++; }
inline void perftData::increaseCheckmateCountAt(int depth) { _checkmateCount[depth]++; }

inline void perftData::setFirstMoveAt(int firstMoveIndex, model::Move currentMove) 
{
    _firstMoves[firstMoveIndex] = currentMove;
}

inline void perftData::increaseNodeCountPerFirstMoveAt(int firstMoveIndex) 
{
    _nodeCountPerFirstMove[firstMoveIndex]++;
}

} // namespace engine