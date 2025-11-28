#include "engine/pickmove/search_memory.h"

#include "model/position/piece_type.h"

namespace engine {
    
SearchMemory::SearchMemory(int maxDepth) : max_depth_(maxDepth) 
{
    castle_rights_.resize(max_depth_ + 1);
    ep_target_masks_.resize(max_depth_ + 1);
    no_captures_or_pawn_moves_counts_.resize(max_depth_ + 1);

    for (int i = 0; i <= max_depth_; i++) {
        castle_rights_[i] = 0b1111;
        ep_target_masks_[i] = 0ULL;
        no_captures_or_pawn_moves_counts_[i] = 0;
    }
}

void SearchMemory::remove_castle_rights_for_remaining_depths(
    int current_depth,
    unsigned char rights_to_remove) 
{
    for (int i = current_depth + 1; i < max_depth_; i++) {
        castle_rights_[i] &= ~rights_to_remove;
    }
}

void SearchMemory::restore_castle_rights_for_remaining_depths(int current_depth) 
{
    for (int i = current_depth + 1; i < max_depth_; i++) {
        castle_rights_[i] = castle_rights_[current_depth];
    }
}

void SearchMemory::set_castle_rights(
    int current_depth, 
    const model::Move& move, 
    bool is_w, 
    model::Piece::Type moved_piece_type) 
{
    if (move.is_any_castle()) {
        remove_castle_rights_for_remaining_depths(
            current_depth, 
            is_w ? w_both_sides_castle_rights : b_both_sides_castle_rights
        );
    }

    if (moved_piece_type == model::Piece::Type::W_KING || moved_piece_type == model::Piece::Type::B_KING) {
        if (is_w) {
            if (castle_rights_[current_depth] & w_both_sides_castle_rights)
                remove_castle_rights_for_remaining_depths(current_depth, w_both_sides_castle_rights);
        } else {
            if (castle_rights_[current_depth] & b_both_sides_castle_rights)
                remove_castle_rights_for_remaining_depths(current_depth, b_both_sides_castle_rights);
        }
    }

    if (moved_piece_type == model::Piece::Type::W_ROOK || moved_piece_type == model::Piece::Type::B_ROOK) {
        if (is_w) {
            if (move.get_from_sq() == 0) {
                if (castle_rights_[current_depth] & w_kside_castle_rights)
                    remove_castle_rights_for_remaining_depths(current_depth, w_kside_castle_rights);
            } else if (move.get_from_sq() == 7) {
                if (castle_rights_[current_depth] & w_qside_castle_rights)
                    remove_castle_rights_for_remaining_depths(current_depth, w_qside_castle_rights);
            }
        } else {
            if (move.get_from_sq() == 56) {
                if (castle_rights_[current_depth] & b_kside_castle_rights)
                    remove_castle_rights_for_remaining_depths(current_depth, b_kside_castle_rights);
            } else if (move.get_from_sq() == 63) {
                if (castle_rights_[current_depth] & b_qside_castle_rights)
                    remove_castle_rights_for_remaining_depths(current_depth, b_qside_castle_rights);
            }
        }
    }
}

void SearchMemory::unset_castle_rights(int current_depth) 
{
    if (castle_rights_[current_depth] != castle_rights_[current_depth + 1]) {
        restore_castle_rights_for_remaining_depths(current_depth);
    }
}

void SearchMemory::override_castle_rights(unsigned char rights) 
{
    for (int i = 0; i < max_depth_; i++) {
        castle_rights_[i] = rights;
    }
}

void SearchMemory::handle_no_capture_count(
    const model::Move& move, 
    int current_depth, 
    model::Piece::Type  moved_piece_type)
{
    // If the move is a capture, reset the no capture count
    if (move.is_any_capture()) {
        reset_no_captures_or_pawn_moves_count_at_depth(current_depth + 1);
        return;
    }

    // If the move is a pawn move, reset the no capture count
    if (moved_piece_type == model::Piece::Type::W_PAWN || moved_piece_type == model::Piece::Type::B_PAWN) {
        reset_no_captures_or_pawn_moves_count_at_depth(current_depth + 1);
        return;
    }

    // If the move is not a capture or pawn move, increment the no capture count
    increment_no_captures_or_pawn_moves_at_depth(current_depth + 1);
}

void SearchMemory::handle_ep_memory(
    const model::Move& move, 
    bool is_w, 
    int current_depth, 
    sq_idx to_sq) 
{
    if (not move.is_double_pawn_push()) {
        set_ep_target_at_depth(current_depth + 1, 0ULL);
        return;
    }

    if (move.is_double_pawn_push()) {
        bitmask ep_target_mask = is_w ? (1ULL << (to_sq - 8)) 
                                          : (1ULL << (to_sq + 8));

        set_ep_target_at_depth(current_depth + 1, ep_target_mask);
        
        // FIXME: Temporary because I don't know how to implement this haha
        // z_hasher_.hash_ep_file(to_sq % 8);
    }
}

} // namespace engine