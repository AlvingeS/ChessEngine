#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {

int ChessUtils::manhattanDistance(int i, int j) 
{
    int rankDiff = rankFromBitIndex(i) - rankFromBitIndex(j);
    int fileDiff = fileFromBitIndex(i) - fileFromBitIndex(j);
    return abs(rankDiff) + abs(fileDiff);
}

}