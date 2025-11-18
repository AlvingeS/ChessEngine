#pragma once

#include "model/position/board.h"

#include <string>
namespace io {

struct Fen {
    
    static void setBoardFromFen(const std::string& fen, model::Bitboards& bitboards, model::StateBitmasks& stateBitmasks, model::PieceMap& pieceMap);
    static std::string getFenFromBoard(const model::PieceMap& pieceMap);

};

} // namespace io