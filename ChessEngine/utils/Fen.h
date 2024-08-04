#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/board/SquaresLookup.h"


namespace utils {

void setBoardFromFen(const std::string& fen, board::BitBoards& bitboards, board::GameStateBitMasks& gameStateBitmasks, board::SquaresLookup& squaresLookup);
std::string& getFenFromBoard(board::SquaresLookup& squaresLookup);

} // namespace utils