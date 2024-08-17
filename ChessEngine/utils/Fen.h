#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/board/SquaresLookup.h"


namespace utils {

void setBoardFromFen(const std::string& fen, board::Bitboards& bitboards, board::GameStateBitmasks& gameStateBitmasks, board::SquaresLookup& squaresLookup);
std::string getFenFromBoard(board::SquaresLookup& squaresLookup);

} // namespace utils