#pragma once

#include "ChessEngine/common.h"

namespace bits {
    constexpr U64 whiteKingSideCastleMask = 0x000000000000006ULL;
    constexpr U64 whiteQueenSideCastleMask = 0x0000000000000070ULL;
    constexpr U64 blackKingSideCastleMask = 0x0600000000000000ULL;
    constexpr U64 blackQueenSideCastleMask = 0x7000000000000000ULL;
}