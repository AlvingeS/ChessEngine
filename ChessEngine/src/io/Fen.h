#pragma once

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/board/SquaresLookup.h"

namespace io {

void setBoardFromFen(const std::string& fen, representation::board::Bitboards& bitboards, representation::board::GameStateBitmasks& gameStateBitmasks, representation::board::SquaresLookup& squaresLookup);
std::string getFenFromBoard(const representation::board::SquaresLookup& squaresLookup);

} // namespace io