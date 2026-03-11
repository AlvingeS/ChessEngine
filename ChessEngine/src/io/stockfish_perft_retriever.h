#pragma once

#include "model/move/move.h"
#include "model/position/piece_map.h"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace io::stockfish
{

std::unordered_map<model::Move, uint64_t> get_perft_results(const std::string& FEN, int depth, const model::PieceMap pm);
std::vector<model::Move> get_legal_moves_from_fen(const std::string& FEN, const model::PieceMap pm);
std::string move_to_str(model::Move move, bool w_started);
std::pair<bool, std::string> compare_first_move_counts_to_stockfish(
    const std::unordered_map<model::Move, uint64_t>& first_move_counts,
    const std::unordered_map<model::Move, uint64_t>& stockfish_results,
    bool w_started
);

} // namespace io::stockfish