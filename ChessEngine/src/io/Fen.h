#pragma once

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"

#include "ChessEngine/src/model/board/SquaresLookup.h"

namespace io {

void setBoardFromFen(const std::string& fen, model::Bitboards& bitboards, model::GameStateBitmasks& gameStateBitmasks, model::SquaresLookup& squaresLookup);
std::string getFenFromBoard(const model::SquaresLookup& squaresLookup);

} // namespace io