namespace logic {

inline std::vector<int>& Containers::getSlidingPiecefreeMovesIndices()
{
    return _slidingPiecefreeMovesIndices;
}

inline std::vector<int>& Containers::getLeapingPiecefreeMovesIndices()
{
    return _leapingPiecefreeMovesIndices;
}

inline std::vector<int>& Containers::getSlidingPieceCapturableMovesIndices()
{
    return _slidingPieceCapturableMovesIndices;
}

inline std::vector<int>& Containers::getLeapingPieceCapturableMovesIndices()
{
    return _leapingPieceCapturableMovesIndices;
}

inline std::vector<int>& Containers::getPiecePositionIndices()
{
    return _piecePositionIndices;
}

} // namespace logic