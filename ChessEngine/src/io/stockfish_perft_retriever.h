#pragma once

#include "model/move/move.h"
#include "model/position/piece_map.h"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace io::stockfish
{

std::unordered_map<model::Move, uint64_t> get_perft_results(const std::string& FEN, int depth, const model::PieceMap pm);


} // namespace io::stockfish