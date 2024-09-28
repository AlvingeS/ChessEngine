#pragma once

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"

namespace utils {

void setBoardFromFen(const std::string& fen, game::board::Bitboards& bitboards, game::board::GameStateBitmasks& gameStateBitmasks, game::board::SquaresLookup& squaresLookup);
std::string getFenFromBoard(const game::board::SquaresLookup& squaresLookup);

} // namespace utils