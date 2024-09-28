#pragma once

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"

namespace utils {

void setBoardFromFen(const std::string& fen, board::Bitboards& bitboards, board::GameStateBitmasks& gameStateBitmasks, board::SquaresLookup& squaresLookup);
std::string getFenFromBoard(const board::SquaresLookup& squaresLookup);

} // namespace utils