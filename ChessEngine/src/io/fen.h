#pragma once

#include "model/position/position.h"

#include <string>
namespace io::fen {

void set_board_from_fen(const std::string& fen, model::Position& pos);
std::string get_fen_from_board(const model::PieceMap& piece_map);

} // namespace io::fen