#include "ZHasher.h"

#include <iostream>
#include <random>

namespace board {

ZHasher::ZHasher()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    // Generate random numbers for board/piece table
    for (size_t i = 0; i < 64; i++) {
        for (size_t j = 0; j < 12; j++) {
            _randBoardPieceTypeNums[i][j] = dis(gen);
        }
    }

    // Generate random numbers for castle rights table
    for (size_t i = 0; i < 4; i++) {
        _randCastleRightsNums[i] = dis(gen);
    }

    // Generate random numbers for en passant file table
    for (size_t i = 0; i < 8; i++) {
        _randEnPassantFileNums[i] = dis(gen);
    }

    // Generate random number for is white table
    _randIsWhiteNum = dis(gen);
}

} // namespace board