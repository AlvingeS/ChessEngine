#pragma once

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"

#include "ChessEngine/src/model/board/PieceMap.h"

namespace io {

void setBoardFromFen(const std::string& fen, model::Bitboards& bitboards, model::StateBitmasks& stateBitmasks, model::PieceMap& pieceMap);
std::string getFenFromBoard(const model::PieceMap& pieceMap);

} // namespace io