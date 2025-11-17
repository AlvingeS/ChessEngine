#pragma once

#include "common.h"
#include <unordered_map>

namespace io::stockfish
{

std::unordered_map<std::string, uint64_t> getPerftResults(const std::string& FEN, int depth);


} // namespace io::stockfish