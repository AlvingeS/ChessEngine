#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>

namespace io::stockfish
{

std::unordered_map<std::string, uint64_t> get_perft_results(const std::string& FEN, int depth);


} // namespace io::stockfish