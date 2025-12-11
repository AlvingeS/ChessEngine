#pragma once

#include "model/position/position.h"

#include <string>
namespace io::fen {

void set_pos_from_fen(const std::string& fen, model::Position& pos);

} // namespace io::fen