#pragma once

#include "model/position/board.h"

#include <string>
namespace io {

struct Fen {
    
    static void setBoardFromFen(const std::string& fen, model::Bitboards& bitboards, model::StateBitmasks& state_bitmasks, model::PieceMap& piece_map);
    static std::string getFenFromBoard(const model::PieceMap& piece_map);

};

} // namespace io