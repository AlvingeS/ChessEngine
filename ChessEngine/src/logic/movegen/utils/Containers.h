#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace movegen {
namespace utils {

class Containers {

public:
    static void init();

    inline static std::vector<int>& getSlidingPiecefreeMovesIndices()
    {
        return _slidingPiecefreeMovesIndices;
    }

    inline static std::vector<int>& getLeapingPiecefreeMovesIndices()
    {
        return _leapingPiecefreeMovesIndices;
    }

    inline static std::vector<int>& getSlidingPieceCapturableMovesIndices()
    {
        return _slidingPieceCapturableMovesIndices;
    }

    inline static std::vector<int>& getLeapingPieceCapturableMovesIndices()
    {
        return _leapingPieceCapturableMovesIndices;
    }

    inline static std::vector<int>& getPiecePositionIndices()
    {
        return _piecePositionIndices;
    }

private:
    static std::vector<int> _slidingPiecefreeMovesIndices;
    static std::vector<int> _leapingPiecefreeMovesIndices;
    
    static std::vector<int> _slidingPieceCapturableMovesIndices;
    static std::vector<int> _leapingPieceCapturableMovesIndices;

    static std::vector<int> _piecePositionIndices;

};

} // namespace utils
} // namespace movegen
} // namespace logic