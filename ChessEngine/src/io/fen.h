#pragma once

#include "model/position/board.h"

#include <string>
namespace io {

struct Fen {
    
    static void set_board_from_fen(const std::string& fen, model::Bitboards& bitboards, model::OccupancyMasks& occupancy_masks, model::PieceMap& piece_map);
    static std::string get_fen_from_board(const model::PieceMap& piece_map);

};

} // namespace io