#pragma once

#include "logic/masks.h"

#include "model/types.h"
#include "model/position/piece_type.h"
#include "model/move/move.h"

#include <vector>

namespace logic::utils 
{

// Old logic::bits
bool get_bit(bitmask mask, int i);
int lsb_idx(bitmask mask); 
int msb_idx(bitmask mask);
void get_bit_idxs(std::vector<int>& idxs, bitboard bb);
int pop_count(bitmask mask);
void print_bitmask_in_bits(bitmask mask);   

// Old logic::chess_utils
bitmask get_file_mask(int file);
bitmask get_rank_mask(int rank);
int rank_from_sq(sq_idx sq);
int file_from_sq(sq_idx sq);
int abs(int n);
int manhattan_distance(int i, int j);

// Old logic::move_utils
model::Piece::Type get_promotion_piece_type(int promotionFlag, bool is_w);
int determine_capture_sq(const model::Move& move, bool is_w);

struct MoveResult {
    model::Piece::Type captured_piece_type;
    model::Piece::Type moved_piece_type;

    MoveResult() {
        captured_piece_type = model::Piece::Type::EMPTY;
        moved_piece_type    = model::Piece::Type::EMPTY;
    }
};

} // namespace logic::utils
#include "logic/utils.inl"