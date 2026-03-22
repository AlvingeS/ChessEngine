#pragma once

#include "logic/masks.h"

#include "model/types.h"
#include "model/move/move.h"

#include <vector>

namespace logic::utils 
{

// Old logic::bits
void set_bit(bitmask& mask, sq_t i);
bool get_bit(bitmask mask, sq_t i);
int lsb_idx(bitmask mask); 
int msb_idx(bitmask mask);
int pop_count(bitmask mask);

// Old logic::chess_utils
bitmask get_file_mask(int file);
bitmask get_rank_mask(int rank);
int rank_from_sq(sq_t sq);
int file_from_sq(sq_t sq);

// Old logic::move_utils
PieceType get_promotion_piece_type(int promo_flag, bool is_w);
int determine_capture_sq(const model::Move& move, bool is_w);

template<typename Func>
inline void controlled_for_each_bit(bitmask mask, Func f);

template<typename Func>
inline void for_each_bit(bitmask mask, Func f);

} // namespace logic::utils
#include "logic/utils.inl"