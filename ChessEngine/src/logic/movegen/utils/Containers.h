#pragma once

#include <vector>

namespace logic {

class Containers {

public:
    static void init();

    static std::vector<int>& getSlidingPiecefreeMovesIndices();
    static std::vector<int>& getLeapingPiecefreeMovesIndices();
    static std::vector<int>& getSlidingPieceCapturableMovesIndices();
    static std::vector<int>& getLeapingPieceCapturableMovesIndices();
    static std::vector<int>& getPiecePositionIndices();

private:
    static std::vector<int> _slidingPiecefreeMovesIndices;
    static std::vector<int> _leapingPiecefreeMovesIndices;
    
    static std::vector<int> _slidingPieceCapturableMovesIndices;
    static std::vector<int> _leapingPieceCapturableMovesIndices;

    static std::vector<int> _piecePositionIndices;
};

} // namespace logic