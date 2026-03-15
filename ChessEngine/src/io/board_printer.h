#pragma once

#include "model/types.h"
#include "model/position/position.h"
#include "model/move/move.h"

#include<optional>
#include <vector>

namespace io {

struct DebugInfo {
    bitmask occ_mask = 0ULL;
    int move_val;
};

class BoardPrinter {

public:
    BoardPrinter(const model::Position& pos);
    void print(std::optional<model::Move> move = std::nullopt);
    
private:
    void fill_boards();
    bool is_valid_piece(model::Piece::Type piece_type);
    
    const model::Position& pos_;
    std::vector<std::vector<char>> board_bbs_;
    std::vector<std::vector<char>> board_pm_;
};

} // namespace io