#include "ChessEngine/src/logic/movegen/utils/Containers.h"

namespace logic {

std::vector<int> Containers::_slidingPiecefreeMovesIndices;
std::vector<int> Containers::_leapingPiecefreeMovesIndices;
std::vector<int> Containers::_slidingPieceCapturableMovesIndices;
std::vector<int> Containers::_leapingPieceCapturableMovesIndices;
std::vector<int> Containers::_piecePositionIndices;

void Containers::init() {
    _slidingPiecefreeMovesIndices = std::vector<int>(32, 0);
    _leapingPiecefreeMovesIndices = std::vector<int>(8, 0);

    _slidingPieceCapturableMovesIndices = std::vector<int>(32, 0);
    _leapingPieceCapturableMovesIndices = std::vector<int>(8, 0);

    _piecePositionIndices = std::vector<int>(16, 0);
}

std::vector<int>& Containers::getSlidingPiecefreeMovesIndices()
{
    return _slidingPiecefreeMovesIndices;
}

std::vector<int>& Containers::getLeapingPiecefreeMovesIndices()
{
    return _leapingPiecefreeMovesIndices;
}

std::vector<int>& Containers::getSlidingPieceCapturableMovesIndices()
{
    return _slidingPieceCapturableMovesIndices;
}

std::vector<int>& Containers::getLeapingPieceCapturableMovesIndices()
{
    return _leapingPieceCapturableMovesIndices;
}

std::vector<int>& Containers::getPiecePositionIndices()
{
    return _piecePositionIndices;
}

} // namespace logic